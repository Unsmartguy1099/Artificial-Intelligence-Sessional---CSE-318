package mypackage;

import mypackage.*;
public class Edge {
    int u;
    int v;
    int w;
    Edge(int u,int v,int w){
        this.u=u;
        this.v=v;
        this.w=w;
    }

    Edge(Edge edge){
        this.u=edge.u;
        this.v=edge.v;
        this.w=edge.w;
    }
}
