#include "node.h"

vector<RoutingNode*> distanceVectorNodes;

void routingAlgo(vector<RoutingNode*> distanceVectorNodes);



int main() {
    int n; // number of nodes
    cin>>n;
    string name; //Node label
    distanceVectorNodes.clear();
    for (int i = 0 ; i < n; i++) {
      RoutingNode *newnode = new RoutingNode();
      cin>>name;
      newnode->setName(name);
      distanceVectorNodes.push_back(newnode);
    }
    cin>>name;
    /*
      For each node label(@name), it's own ip address, ip address of another node
      defined by @oname will be inserted in the node's own datastructure interfaces 
    */
    while(name != "EOE") { //End of entries
      for (int i =0 ; i < distanceVectorNodes.size(); i++) {
  string myeth,oeth, oname;
  if(distanceVectorNodes[i]->getName() == name) {
    //node interface ip
    cin>>myeth;   
    //ip of another node connected to myeth (nd[i])
    cin>>oeth;
    //label of the node whose ip is oeth
    cin>>oname;
    for(int j = 0 ; j < distanceVectorNodes.size(); j++) {
      if(distanceVectorNodes[j]->getName() == oname) {
        /*
    @myeth: ip address of my (distanceVectorNodes[i]) end of connection.
    @oeth: ip address of other end of connection.
    @distanceVectorNodes[j]: pointer to the node whose one of the interface is @oeth
        */
        distanceVectorNodes[i]->addInterface(myeth, oeth, distanceVectorNodes[j]);
        //Routing table initialization
        /*
    @myeth: ip address of my (distanceVectorNodes[i]) ethernet interface.
    @0: hop count, 0 as node does not need any other hop to pass packet to itself.
    
        */
        distanceVectorNodes[i]->addTblEntry(myeth, 0);
        break;
      }
    }
  }
      }
      cin>>name;
    }

    cout<<"Before breaking B and C connection"<<endl;

    /* The logic of the routing algorithm should go here */
    routingAlgo(distanceVectorNodes);
    /* Add the logic for periodic update (after every 1 sec) here */
    // removing all the enteries of c from interface list of B and making routing table enteries for c as 16

    for(int k3=0;k3<(distanceVectorNodes[1]->getTable()).tbl.size();k3++)
      {
        if((distanceVectorNodes[1]->getTable()).tbl[k3].dstip== "10.0.1.3")
        {
          distanceVectorNodes[1]->updateCost(16,k3);
        }
      }

    
     for(int k3=0;k3<(distanceVectorNodes[2]->getTable()).tbl.size();k3++)
      {
        if((distanceVectorNodes[2]->getTable()).tbl[k3].dstip== "10.0.0.21")
        {
          distanceVectorNodes[2]->updateCost(16,k3);
        }
      }


     for(int k3=0;k3<(distanceVectorNodes[2]->getTable()).tbl.size();k3++)
      {
        if((distanceVectorNodes[2]->getTable()).tbl[k3].dstip== "10.0.1.23")
        {
          distanceVectorNodes[2]->updateCost(16,k3);
        }
      }

      cout<<"________________________________________________________________________________________________"<<endl;

      cout<<"After breaking B and C connection"<<endl;


    routingAlgo(distanceVectorNodes);

    cout<<"________________________________________________________________________________________________"<<endl;
    
}

