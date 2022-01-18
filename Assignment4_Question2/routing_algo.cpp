#include "node.h"
#include <iostream>

using namespace std;

void printRT(vector<RoutingNode*> nd){
/*Print routing table entries*/
  for (int i = 0; i < nd.size(); i++) {
    nd[i]->printTable();
  }
}

void check_connections(vector<RoutingNode*> nd)
{
  for(int k=0;k<nd.size();k++)
  {
    for(int k3=0;k3<(nd[k]->getTable()).tbl.size();k3++)
    {
      if(nd[k]-> CompareCost(16,k3))
      {
        //cout<<"updateTblEntry got called for : "<< (nd[k]->getTable()).tbl[k3].dstip<< " " <<endl;
        nd[k]-> updateTblEntry((nd[k]->getTable()).tbl[k3].dstip);
      }
    }
  }
}

void routingAlgo(vector<RoutingNode*> nd){
  
  int num=nd.size();
  vector<struct routingtbl> nd_copy;

  check_connections(nd);

  while(true)
  {

    nd_copy.clear();

    //copying the node list, inorder to check convergence after the nodes have sent message to their neighbors

    for(int j=0;j<num;j++)
    {
      nd_copy.push_back(nd[j]->getTable());
    }

    //All the nodes send message to their neighbors

    for(int i=0;i<nd.size();i++)
    {
      printRT(nd);
      cout<<"############################################"<<endl;
      nd[i]->sendMsg();
      check_connections(nd);
    }

  //The Loop checks for convergence

  bool is_changed= false;

  for(int k=0;k<num;k++)
  {
     if(nd_copy[k].tbl.size()!=(nd[k]->getTable()).tbl.size())
     {
       is_changed= true;
       break;
     }
     for(int k3=0;k3<(nd[k]->getTable()).tbl.size();k3++)//compares the current and previous table
     {
       if(nd_copy[k].tbl[k3].dstip !=(nd[k]->getTable()).tbl[k3].dstip || nd_copy[k].tbl[k3].nexthop !=(nd[k]->getTable()).tbl[k3].nexthop || nd_copy[k].tbl[k3].ip_interface !=(nd[k]->getTable()).tbl[k3].ip_interface || nd_copy[k].tbl[k3].cost !=(nd[k]->getTable()).tbl[k3].cost)
       {
         is_changed= true;
       }

     }
  }

  if(is_changed== false)
  {
    break;
  }



  }

  check_connections(nd);

  cout<<"***********************************************"<<endl;

  for(int i = 0; i<nd.size(); i++)
  {
    nd[i]->printTable();
  }

  cout<<"***********************************************"<<endl;
}


struct RoutingEntry make_newEntry(RouteMsg *msg, int index)
{
  RoutingEntry rt;
  rt.ip_interface= msg->recvip;
  rt.nexthop=msg->from;
  rt.dstip=msg->mytbl->tbl[index].dstip;
  rt.cost=msg->mytbl->tbl[index].cost+1;

  return rt;

}


void RoutingNode::recvMsg(RouteMsg *msg) 
{
  for(int i=0;i<msg->mytbl->tbl.size();i++)
  {
    int is_present=0;

    // checking if reciver table has entry whose dsip is same as dsip of any of the enteries in the message table, if true then comparison is made
    //inorder to check if already saved is small or the new value is small, which every is small, that one is updated as the cost

    for(int j=0;j<mytbl.tbl.size();j++)
    {
      if(mytbl.tbl[j].dstip==msg->mytbl->tbl[i].dstip)
      {
        //Bellman-ford condition applied here
        if(mytbl.tbl[j].cost > msg->mytbl->tbl[i].cost+1 || mytbl.tbl[j].nexthop==msg->from)
        {
          mytbl.tbl[j].cost=msg->mytbl->tbl[i].cost+1;
          mytbl.tbl[j].nexthop = msg->from;
        }
        is_present++;
        break;
      }
    }

    //if reciver table does not have entry whose dsip is same as dsip of any of the enteries in the message table, then a new entry is made in reciever's table 
    if(is_present==0)
    {
      RoutingEntry rt= make_newEntry(msg,i);
      mytbl.tbl.push_back(rt);
    }

  }
}