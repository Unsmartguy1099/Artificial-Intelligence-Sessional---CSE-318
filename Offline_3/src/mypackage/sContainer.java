package mypackage;

import java.util.ArrayList;
import java.util.List;
import mypackage.*;

public class sContainer {
    List<Integer> S;
    List<Integer> Sb;
    int iteration;
    sContainer(List<Integer> iS,List<Integer> iSb){
        S=new ArrayList<Integer>();
        Sb=new ArrayList<Integer>();
        iteration=0;

        for (int i=0;i<iS.size();i++)
            S.add(iS.get(i));
        for(int i=0;i<iSb.size();i++)
            Sb.add(iSb.get(i));
    }
}
