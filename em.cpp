#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <sys/time.h>

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

vector<string> output;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

public:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<long double> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

	map<string,int> val_ind;
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals,map<string,int> a)
	{
		Node_Name=name;	
		nvalues=n;
		values=vals;
		val_ind=a;

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
	vector<long double> get_CPT()
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
	void set_CPT(vector<long double> new_CPT)
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

 int cnt=0;
 // The whole network represted as a list of nodes
class network{

public:
    vector <Graph_Node> Pres_Graph;
    map<string,int> hash_node;
   
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		hash_node[node.get_name()]=cnt;
		cnt++;
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        // vector<Graph_Node>::iterator listIt;
        // int count=0;
        // for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        // {
        //     if(listIt->get_name().compare(val_name)==0)
        //         return count;
        //     count++;
        // }
        return hash_node[val_name];
    }
// get the node at nth index
    Graph_Node* get_nth_node(int n)
    {
       // vector<Graph_Node>::iterator listIt;
       //  int count=0;
       //  for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
       //  {
       //      if(count==n)
       //          return listIt;
       //      count++;
       //  }
       //  return listIt; 
    	return &Pres_Graph[n];
    }
    //get the iterator of a node with a given name
    Graph_Node* search_node(string val_name)
    {
        // vector<Graph_Node>::iterator listIt;
        // for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        // {
        //     if(listIt->get_name().compare(val_name)==0)
        //         return listIt;
        // }
    
        //     cout<<"node not found\n";
        // return listIt;
        return &Pres_Graph[hash_node[val_name]];
    }
	

};

network read_network(char * file)
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile(file); 
  	string temp;
  	string name;
  	vector<string> values;
    map<string,int> tmp;
  	int p=0;
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		////////////
      		output.push_back(line);
      		//////////////
      		ss.str(line);
     		ss>>temp;     		
     		if(temp.compare("variable")==0)
     		{
                    ss>>name;
     				getline (myfile,line);
     				////////////
      				output.push_back(line);
      				//////////////                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{     					
     					ss2>>temp;     					
     				}
     				values.clear();
     				tmp.clear();
     				p=0;
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					tmp[temp]=p;
     					p++;	    					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values,tmp);
     				int pos=Alarm.addNode(new_node);
     		}
     		else if(temp.compare("probability")==0)
     		{
	  				ss>>temp;
     				ss>>temp;
                    Graph_Node* listIt;
                    Graph_Node* listIt1;
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
     				////////////
      				output.push_back(line);
      				//////////////
     				stringstream ss2;
       				ss2.str(line);
     				ss2>> temp;
       				ss2>> temp;
       				vector<long double> curr_CPT;
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
int **data;
int *miss_index;
long double ** prob;

void read_dat(network* net,char * file)
{
    string f(file);
    ifstream record(file);
   // cerr << "asasa"<< endl;
    int miss=-1;
    string tokens;
    int t=0;
    
    while(record.good())
    {
            
            string st;
            getline(record,st);
           stringstream ss(st);
            //cout << "st "<< st<< endl;
            //tokens=strtok((char*)st.c_str()," ");
           // cout << "token 1 "<< tokens << endl;
            t=0;
            miss=-1;
            while(ss>>tokens)
            {
               //cerr << " sds "<<tokens<< endl;
                //if(strcmp(tokens,"\"?\"")==0 || strcmp(tokens,"\"?\"\n")==0)
              //cout << tokens << endl;
                if(tokens.compare("\"?\"") == 0)
                {
                    miss=t;
                    data[line_count][t]=-1;

                }
                else{
                    string str(tokens);
                    data[line_count][t]=net->Pres_Graph[t].val_ind[str];
                }
                //tokens=strtok(NULL," ");
                //cout << "token 2 "<< (void *)tokens<< endl;
                t++;
                
            }
            //cout << "out "<< endl;
            if(miss==-1)
            {
                miss_index[line_count]=-1;
                prob[line_count]=(long double*)malloc(sizeof(long double));
                line_count++;
                cout << "here "<<line_count<< endl;
            }
            else
            {
                prob[line_count]=(long double*)malloc(sizeof(long double)*(net->get_nth_node(miss)->nvalues));
                miss_index[line_count]=miss;
                line_count++;

            }
            //cerr << "end"<< endl;
    }
    record.close();
    cout << "file closed"<< endl;

}



void print_data()
{
    int l=line_count;

    for (int i = 0; i < l; i++)
    {
    	cout << "i "<< i << endl;
        int k=37;
        for (int j = 0; j < k; j++)
        {
            cout << data[i][j]<< " ";
        }
        cout << endl;
    }
}

void init_cpt(network *a)
{


        vector<Graph_Node>::iterator It;
        for(It=a->Pres_Graph.begin();It!=a->Pres_Graph.end();It++)
        {
            int var=It->nvalues;
          //  cerr << var << endl;
            int var1=var;
            std::vector<string> par=It->get_Parents();
            int l=par.size();
            for(int i=0;i<l;i++)
            {
                var*=(a->search_node(par[i]))->get_nvalues();
            }
            vector<long double> cpt;
            cpt.clear();
            cpt.resize(var,1.0);
            // for (int i = 0; i < cpt.size(); ++i)
            // {
            //     cout << "cpt " << cpt[i]<< endl;
            // }
            var=var/var1;
            //cerr << "final assign "<<var << endl;
            //cerr << var1<< endl;
            int sum[var];
            for(int i=0;i<var;i++)
                sum[i]=0;
            for(int i=0;i<(var*var1);i++)
            {
                cpt[i]+=rand()%10;
                // cerr <<"var "<< var << " "<< i <<" "<< cpt.size()<< endl;
                // int j=i%var;
                // cerr << "asaasa "<< j << endl;
                sum[i%var] += cpt[i];
            }
            for(int i=0;i<var*var1;i++)
            {
                cpt[i]/=sum[i%var];
            }
            //cerr << "here"<< endl;
            It->set_CPT(cpt);

        } 
        for(It=a->Pres_Graph.begin();It!=a->Pres_Graph.end();It++)
        {
            std::vector<long double> v=It->get_CPT();
            int l=v.size();
            for(int i=0;i<l;i++)
            {
                cout << v[i] << " ";
            }
            cout << endl;
        }



}

void set_weights(network *net)
{
	for(int i=0;i<line_count;i++)
	{
		if(miss_index[i]==-1)
			prob[i][0]=1;
		else
		{
			Graph_Node * missing=net->get_nth_node(miss_index[i]);
			long double sum=0;
			for(int k=0;k<missing->nvalues;k++)
			{
				data[i][miss_index[i]]=k;
				long double tmp=1;
				int index=data[i][miss_index[i]];
				int l=missing->Parents.size();
				for(int j=0;j<l;j++)
				{
					index=index*(net->Pres_Graph[net->hash_node[missing->Parents[j]]].nvalues)+data[i][net->hash_node[missing->Parents[j]]];
				}
				tmp*=missing->CPT[index];
				int m=missing->Children.size();
				for(int o=0;o<m;o++)
				{

					index=data[i][missing->Children[o]];
					l=net->Pres_Graph[missing->Children[o]].Parents.size();
					for(int j=0;j<l;j++)
					{
						index=index*(net->Pres_Graph[net->hash_node[net->Pres_Graph[missing->Children[o]].Parents[j]]].nvalues)+data[i][net->hash_node[net->Pres_Graph[missing->Children[o]].Parents[j]]];
					}
					tmp*=net->Pres_Graph[missing->Children[o]].CPT[index];
					

				}
				prob[i][k]=tmp;
				sum+=tmp;
			}
			data[i][miss_index[i]]=-1;
      //cout << sum << " sum "<< endl;
			for(int k=0;k<missing->nvalues;k++)
				prob[i][k]/=sum;
		}

	}
}

int check(int example, int node,network* net)
{
	int l=net->Pres_Graph[node].Parents.size();
	for(int i=0;i<l;i++)
	{
		if(miss_index[example]==net->hash_node[net->Pres_Graph[node].Parents[i]])
			return i;
	}
	return -1;
}

void set_cpt(network *net)
{
	int l=net->Pres_Graph.size();
	for(int i=0;i<l;i++)
	{
		//cout << "i "<< i << endl;
		int m=net->Pres_Graph[i].Parents.size();
		int n=1;
		for(int j=0;j<m;j++)
		{
			n*=net->Pres_Graph[net->hash_node[net->Pres_Graph[i].Parents[j]]].nvalues;
		}

		/// POTENTIAL INEFFICIENCIES BOTH SPACE AND TIME
		long double tmp[n];
		for(int p=0;p<n;p++)
			tmp[p]=0.000001*net->Pres_Graph[i].nvalues;
		long double CPT_new[n*net->Pres_Graph[i].nvalues];
		for(int p=0;p<n*net->Pres_Graph[i].nvalues;p++)
			CPT_new[p]=0.000001*1.0;
		for(int a=0;a<line_count;a++)
		{
			int in=check(a,i,net);
			if(miss_index[a]!=i && in==-1)
			{
				int index=data[a][i];
				for(int j=0;j<m;j++)
				{
					index=index*(net->Pres_Graph[net->hash_node[net->Pres_Graph[i].Parents[j]]].nvalues)+data[a][net->hash_node[net->Pres_Graph[i].Parents[j]]];
				}
				CPT_new[index]+=1;
				tmp[index%n]+=1;

			}
			else if(in!=-1)
			{
				int np=1;
				for(int j=in+1;j<m;j++)
				{
					np*=net->Pres_Graph[net->hash_node[net->Pres_Graph[i].Parents[j]]].nvalues;
				}

				int index=data[a][i];
				for(int j=0;j<m;j++)
				{
					if(data[a][net->hash_node[net->Pres_Graph[i].Parents[j]]]==-1){
						index=index*(net->Pres_Graph[net->hash_node[net->Pres_Graph[i].Parents[j]]].nvalues);
						continue;
					}
					index=index*(net->Pres_Graph[net->hash_node[net->Pres_Graph[i].Parents[j]]].nvalues)+data[a][net->hash_node[net->Pres_Graph[i].Parents[j]]];
				}
				for(int op=0;op<net->Pres_Graph[miss_index[a]].nvalues;op++)
				{
					CPT_new[op*np+index]+=prob[a][op];
					tmp[(op*np+index)%n]+=prob[a][op];
				}
			}
			else
			{
				int index=0;
				for(int j=0;j<m;j++)
				{
					index=index*(net->Pres_Graph[net->hash_node[net->Pres_Graph[i].Parents[j]]].nvalues)+data[a][net->hash_node[net->Pres_Graph[i].Parents[j]]];
				}
				
				for(int op=0;op<net->Pres_Graph[i].nvalues;op++)
				{
					CPT_new[op*n+index]+=prob[a][op];
					tmp[index]+=prob[a][op];
				}

			}


		}
		for(int g=0;g<n*net->Pres_Graph[i].nvalues;g++)
		{
			//cout << CPT_new[g]<<" "<< tmp[g%n]<< endl;
			CPT_new[g]/=tmp[g%n];
		}
		vector<long double> v(CPT_new,CPT_new+sizeof(CPT_new)/sizeof(CPT_new[0]));
		net->Pres_Graph[i].set_CPT(v);

	}
}


double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}



int main(int  argc, char ** argv)
{
    double start_time=get_wall_time();
    srand(time(NULL));
    network Alarm;
    int size=atoi(argv[1]);
    data=(int**)malloc(sizeof(int*)*(size+2));
    for(int i=0;i<size+2;i++)
    	data[i]=(int*)malloc(sizeof(int)*39);
    miss_index=new int[size+2];
    prob=(long double**)malloc(sizeof(long double*)*(size+2));
    Alarm=read_network(argv[2]);
    cout << size << endl;
    read_dat(&Alarm,argv[3]);
    cout << "line count "<< line_count<< endl;
  
    //print_data();
    init_cpt(&Alarm);
    cout << "after init"<< endl;
   // return 0;
    vector<Graph_Node>::iterator It;
    // for(It=Alarm.Pres_Graph.begin();It!=Alarm.Pres_Graph.end();It++)
    // {
    //     std::vector<long double> v=It->get_CPT();
    //     int l=v.size();
    //     cout << "new line "<< endl;
    //     for(int i=0;i<l;i++)
    //     {
    //         cout << v[i] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << " ------------------------------------------------------------"<< endl;

    int u=0;
    while(u<100){
      cout << ++u << endl;
    set_weights(&Alarm);
    //cout << "after set weights "<< endl;
    set_cpt(&Alarm);
    //cout << "after set cpt"<< endl;
}

	ofstream outfile;
	outfile.open("solved_alarm.bif");
    int out_size = output.size()-1;
    int pres_graph_i = 0;
    for (int i = 0; i < out_size; ++i)
    {
    	if (output[i].find("-1") == std::string::npos)
    		outfile << output[i] << endl;
    	else
    	{
    		outfile << "\ttable ";
    		std::vector<long double> v= Alarm.Pres_Graph[pres_graph_i].get_CPT();
	        int l=v.size();
	        for(int j=0;j<l;j++)
	        {
	            outfile << roundf(v[j]*10000)/10000 << " ";
	        }
	        outfile << ";" << endl;
	        pres_graph_i++;
    	}
    }
    outfile << output[out_size];
    outfile.close();
    // for(It=Alarm.Pres_Graph.begin();It!=Alarm.Pres_Graph.end();It++)
    // {
    //     std::vector<long double> v=It->get_CPT();
    //     int l=v.size();
    //     cout << "new line "<< endl;
    //     for(int i=0;i<l;i++)
    //     {
    //         cout << v[i] << " ";
    //     }
    //     cout << endl;
    // }	

//     cout << line_count<< endl;
// // Example: to do something
    



	cout<<"Perfect! Hurrah! \n";
	
}





