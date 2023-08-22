package mypackage;

import java.io.*;
import java.util.*;
import java.util.HashMap;
import java.util.Map;
import mypackage.*;

public class MaxCut {
    File file ;
    FileReader fileReader;
    String[] numbers;
    static Random rand;
    static List<Edge> list;
    List<Integer> V;
    int Vsize;
    int Esize;

    MaxCut(String inputName,String outputName){
        //create list of edge
        list=new ArrayList<Edge>();
        V=new ArrayList<Integer>();
        //Create output file
        file = new File("./outputfiles/"+outputName);
        try {
            // Create the file
            if (file.createNewFile()) {
                System.out.println("output.txt created");
            } else {
                System.out.println("output.txt already exists");
            }
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
        //Reading from file
        try {
            fileReader = new FileReader(inputName);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
        BufferedReader bufferedReader = new BufferedReader(fileReader);
        String line;
        while (true) {
            try {
                if (!((line = bufferedReader.readLine()) != null)) break;
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            numbers = line.split(" ");
            //extract data in array and variables
            if(numbers.length==2){
                Vsize=Integer.parseInt(numbers[0]);
                Esize=Integer.parseInt(numbers[1]);
            }
            if(numbers.length==3){
                Edge edge=new Edge(Integer.parseInt(numbers[0]),Integer.parseInt(numbers[1]),Integer.parseInt(numbers[2]));
                list.add(edge);
            }
        }
        // Close the BufferedReader
        try {
            bufferedReader.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        //Random initialization
        rand = new Random();
    }
    sContainer semiGreedy(){
        //vars----------------------------
        List<Edge> RCLe;
        List<Integer> X;
        List<Integer> Y;
        List<Edge> U;
        RCLe=new ArrayList<Edge>();
        U=new ArrayList<Edge>();
        X=new ArrayList<Integer>();
        Y=new ArrayList<Integer>();
        //-------------------------------
        for (int i=1;i<=Vsize;i++)
            V.add(i);
        double alpha = rand.nextInt(100001);
        alpha=alpha/100000;
        for (int i=0;i<list.size();i++)
            U.add(list.get(i));

        int Wmin=FindMinEdge(U);
        int Wmax=FindMaxEdge(U);

        double mue=Wmin+alpha*(Wmax-Wmin);

        for(int i=0;i<U.size();i++)
            if(U.get(i).w>=mue)
                RCLe.add(U.get(i));

        int rand2 = rand.nextInt(RCLe.size());

        X.add(RCLe.get(rand2).u);
        Y.add(RCLe.get(rand2).v);

        int per=(int)(((float)(X.size()+Y.size())/((float)V.size()))*100);

        while(checkXUYneV(X,Y,V)){
            if(per!=(int)(((float)(X.size()+Y.size())/((float)V.size()))*100)){
                System.out.println((int)(((float)(X.size()+Y.size())/((float)V.size()))*100)+"%");
                per=(int)(((float)(X.size()+Y.size())/((float)V.size()))*100);
            }

            //variables---------------------------------
            List<Integer> RCLv=new ArrayList<Integer>();
            List<Integer> Vp=new ArrayList<Integer>();
            List<Integer> union = unionOfLists(X,Y);
            //-----------------------------------------
            for(int i=0;i<V.size();i++)
                Vp.add(V.get(i));
            Vp.removeAll(union);

            HashMap<Integer, Integer> sigmaX = new HashMap<>();
            HashMap<Integer, Integer> sigmaY = new HashMap<>();
            int sigX=0;
            int sigY=0;

            for (int i = 0; i < Vp.size(); i++) {
                int currentVp = Vp.get(i);
                sigY = 0;
                sigX = 0;
                for (Edge edge : list) {
                    if (edge.v == currentVp) {
                        if (X.contains(edge.u)) {
                            sigY += edge.w;
                        }
                        if (Y.contains(edge.u)) {
                            sigX += edge.w;
                        }
                    }
                }
                sigmaY.put(currentVp, sigY);
                sigmaX.put(currentVp, sigX);
            }

            if(findMin(sigmaX,Vp)>findMin(sigmaY,Vp))
                Wmin=findMin(sigmaY,Vp);
            else
                Wmin=findMin(sigmaX,Vp);
            if(findMax(sigmaX,Vp)>findMax(sigmaY,Vp))
                Wmax=findMax(sigmaX,Vp);
            else
                Wmax=findMax(sigmaY,Vp);

            mue=Wmin+alpha*(Wmax-Wmin);
            //System.out.println("mue: "+mue);
            for(int i=0;i<Vp.size();i++){
                if(sigmaX.get(Vp.get(i))>=mue || sigmaY.get(Vp.get(i))>=mue)
                    if(sigmaX.get(Vp.get(i))>sigmaY.get(Vp.get(i)))
                        RCLv.add(Vp.get(i));
                    else
                        RCLv.add(Vp.get(i));
            }

            int rand3 = rand.nextInt(RCLv.size());
            for(int i=0;i<RCLv.size();i++)
                if(sigmaX.get(RCLv.get(rand3))>sigmaY.get(RCLv.get(rand3))){
                    if(!X.contains(RCLv.get(rand3)))
                        X.add(RCLv.get(rand3));
                }
                else{
                    if(!Y.contains(RCLv.get(rand3)))
                        Y.add(RCLv.get(rand3));
                }

        }
        sContainer s=new sContainer(X,Y);
        System.out.println("100%");
        return s;

    }

    sContainer LocalSearch(List<Integer> S1,List<Integer> Sb1){

        List<Integer> S=new ArrayList<Integer>();
        List<Integer> Sb=new ArrayList<Integer>();
        for(int i=0;i<S1.size();i++)
            S.add(S1.get(i));
        for(int i=0;i<Sb1.size();i++)
            Sb.add(Sb1.get(i));
        int it=0;
        boolean change=true;
        while(change){
            it++;
            change=false;
            int sigS=0;
            int sigSb=0;
            for(int currentV=1;currentV<=V.size()&& !change;currentV++){
                sigS = 0;
                sigSb = 0;
                for (Edge edge : list) {
                    if (edge.v == currentV) {
                        if (S.contains(edge.u)) {
                            sigSb += edge.w;
                        }
                        if (Sb.contains(edge.u)) {
                            sigS += edge.w;
                        }
                    }
                }

                if(S.contains(S.contains(currentV) && (sigSb-sigS)>0)){
                    S.remove(Integer.valueOf(currentV));
                    Sb.add(currentV);
                    change=true;
                }else if(Sb.contains(currentV) && (sigS-sigSb)>0){
                    Sb.remove(Integer.valueOf(currentV));
                    S.add(currentV);
                    change=true;
                }
            }
        }

        sContainer SS=new sContainer(S,Sb);
        SS.iteration=it;
        return SS;
    }

    private boolean checkXUYneV(List<Integer> X,List<Integer> Y,List<Integer> V) {
        List<Integer> union = unionOfLists(X,Y);
        if(union.size()!=V.size())
            return true;
        else
            return false;
    }

    private static int FindMaxEdge(List<Edge> U) {
        Edge maxE=new Edge(0,0,0);
        for(int i=0;i<U.size();i++){
            if(U.get(i).w>=maxE.w){
                maxE.u=U.get(i).u;
                maxE.v=U.get(i).v;
                maxE.w=U.get(i).w;
            }
        }
        return maxE.w;
    }

    private static int FindMinEdge(List<Edge> U) {
        Edge minE=new Edge(0,0,100000);
        for(int i=0;i<U.size();i++){
            if(U.get(i).w<=minE.w){
                minE.u=U.get(i).u;
                minE.v=U.get(i).v;
                minE.w=U.get(i).w;
            }
        }
        return minE.w;
    }

    private static int FindMaxEdgeI(List<Edge> U) {
        int maxIn=0;
        for(int i=0;i<U.size();i++){
            if(U.get(i).w>U.get(maxIn).w){
               maxIn=i;
            }
        }
        return maxIn;
    }

    private static int findMin(HashMap<Integer,Integer> tempList,List<Integer> Vp){
        if(tempList.size()>0 && Vp.size()>0) {
            int min=tempList.get(Vp.get(0));
            for(int i=0;i<Vp.size();i++)
                if(min>tempList.get(Vp.get(i)))
                    min=tempList.get(Vp.get(i));
            return min;
        }else
            return 0;
    }

    private static int findMax(HashMap<Integer,Integer> tempList,List<Integer> Vp){
        if(tempList.size()>0 && Vp.size()>0) {
            int max=tempList.get(Vp.get(0));
            for(int i=0;i<Vp.size();i++)
                if(max<tempList.get(Vp.get(i)))
                    max=tempList.get(Vp.get(i));
            return max;
        }else
            return 0;
    }

    public static <T> List<T> unionOfLists(List<T> list1, List<T> list2) {
        HashSet<T> set = new HashSet<>(list1);
        set.addAll(list2);
        return new ArrayList<>(set);
    }

    public int calculateCut(List<Integer> S,List<Integer> Sb){
        int cut=0;
        for(int i=0;i<list.size();i++)
            if(S.contains(list.get(i).u) && Sb.contains(list.get(i).v)||S.contains(list.get(i).v) && Sb.contains(list.get(i).u))
                cut=cut+list.get(i).w;
        return cut;
    }

    public sContainer randomized(){
        List<Integer> S=new ArrayList<Integer>();
        List<Integer> Sb=new ArrayList<Integer>();

        for(int i=1;i<Vsize;i++){
            int rand4 = rand.nextInt(2);
            if(rand4==0)
                S.add(i);
            else
                Sb.add(i);
        }

        sContainer ss=new sContainer(S,Sb);
        return ss;
    }

    public sContainer greedy(){

        List<Edge> U=new ArrayList<Edge>();
        List<Integer> S=new ArrayList<Integer>();
        List<Integer> Sb=new ArrayList<Integer>();
        for(int i=0;i<list.size();i++)
            U.add(list.get(i));

        float s=U.size();
        int per=(int)(((s-(float)(U.size()))/s)*100);
        while (U.size()!=0){
            if(per!=(int)(((s-(float)(U.size()))/s)*100)){
                System.out.println((int)(((s-(float)(U.size()))/s)*100)+"%");
                 per=(int)(((s-(float)(U.size()))/s)*100);}

            int i=rand.nextInt(U.size());
                //i=FindMaxEdgeI(U);

            if(!S.contains(U.get(i).u) && !Sb.contains(U.get(i).u)){
                S.add(U.get(i).u);
                int s1=calculateCut(S,Sb);
                S.remove(S.size()-1);
                Sb.add(U.get(i).u);
                int s2=calculateCut(S,Sb);
                if(s1>s2){
                    Sb.remove(Sb.size()-1);
                    S.add(U.get(i).u);
                }
            }
            if(!S.contains(U.get(i).v) && !Sb.contains(U.get(i).v)){
                S.add(U.get(i).v);
                int s1=calculateCut(S,Sb);
                S.remove(S.size()-1);
                Sb.add(U.get(i).v);
                int s2=calculateCut(S,Sb);
                if(s1>s2){
                    Sb.remove(Sb.size()-1);
                    S.add(U.get(i).v);
                }
            }
            U.remove(i);
        }

        System.out.println("100%");
        sContainer ss=new sContainer(S,Sb);
        return ss;
    }

    public int GRASP(int n){
        int max=0;
        for(int i=0;i<n;i++){
            sContainer ss=randomized();
            sContainer local=LocalSearch(ss.S,ss.Sb);
            int cal=calculateCut(local.S,local.Sb);
            if(cal>max)
                max=cal;
        }

        return max;
    }
}
