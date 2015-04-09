#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <bits/stdc++.h>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


 // The whole network represted as a list of nodes
class network{

	list <Graph_Node> Pres_Graph;

public:
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }
	

};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}


int line_count=0;
vector< vector<int> > data;
vector<int> miss_index;
vector<double> prob;

void read_dat(network a)
{
    ifstream record("records.dat");
    //cerr << "asasa"<< endl;
    bool missing=false;
    int miss=-1;
    char * tokens;
    int t=0;
    while(!record.eof())
    {
            
            string st;
            getline(record,st);
            tokens=strtok((char*)st.c_str()," ");
            t=0;
            vector<int> tmp;
            miss=-1;
            missing=false;
            while(tokens!=0)
            {
               //cerr << " sds "<<tokens<< endl;
                if(strcmp(tokens,"\"?\"")==0)
                {
                    missing = true;
                    miss=t;
                    tmp.push_back(-1);

                }
                else{
                    //cerr << "here"<< endl;
                    vector<string> v=(*a.get_nth_node(t)).get_values();
                    int len=v.size();
                    for(int i=0;i<len;i++)
                    {
                        //cerr << tokens<< " "<< v[i].c_str() << endl;
                        if(strcmp(tokens,v[i].c_str())==0)
                        {
                           //cerr << "pushing "<< endl;
                            tmp.push_back(i);
                            break;
                        }
                    }    
                }
                tokens=strtok(NULL," ");
                t++;
                
            }
            if(miss==-1)
            {
                data.push_back(tmp);
                prob.push_back(1);
                miss_index.push_back(-1);
                line_count++;
            }
            else
            {
                vector<string> v=(*a.get_nth_node(miss)).get_values();
                int l=v.size();
                //cerr << "l "<< l << endl;
                for(int i=0;i<l;i++)
                {
                    //cerr << "a"<< miss<< " len" << tmp.size()<< endl;
                    //std::vector<int> tmp1(tmp.begin(),tmp.end());
                    tmp[miss]=i;
                    data.push_back(tmp);
                    miss_index.push_back(miss);
                    prob.push_back(1);
                    line_count++;
                }

            }
            //cerr << "end"<< endl;
    }

}



void print_data()
{
    int l=data.size();

    for (int i = 0; i < l; ++i)
    {
        int k=data[i].size();
        for (int j = 0; j < k; ++j)
        {
            cout << data[i][j]<< " ";
        }
        cout << endl;
    }
}


// int len=a[i].get_values().size();
//                 std::vector<int> temp1=a[i].get_values();
//                 if(strcmp(tokens,"?")==0)
//                     miss_index.push_back(i);
//                 else
//                 {
//                     for(int j=0;j<len;j++)
//                     {
//                         if(strcmp(tokens,temp1[j].c_str())==0)
//                         {
//                             tmp.push_back(j);
//                             break;
//                         }
//                     }
//                 }


int main()
{
    network Alarm;
    Alarm=read_network();
    read_dat(Alarm);
    print_data();
    cout << line_count<< endl;
// Example: to do something
    



	cout<<"Perfect! Hurrah! \n";
	
}





