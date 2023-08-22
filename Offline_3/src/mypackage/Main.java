package mypackage;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import mypackage.*;

public class Main {

    public static void main(String[] args) throws IOException {
        Scanner scanner = new Scanner(System.in);

        String input = new String();

        if (args.length > 0) {
            input = args[0];
        } else {
            System.out.print("Enter file name: ");
            input = scanner.nextLine();
        }

        int it = 5;
        MaxCut maxCut = new MaxCut("./set1/" + input + ".txt", input + "output.txt");

        sContainer randomized = maxCut.randomized();
        //sContainer greedy = maxCut.greedy();
        sContainer semigreedy = maxCut.semiGreedy();
        sContainer local = maxCut.LocalSearch(semigreedy.S, semigreedy.Sb);
        int grasp = maxCut.GRASP(it);

        int max2=0;
        for(int i=0;i<5;i++){
            sContainer greedy = maxCut.greedy();
            max2=max2+maxCut.calculateCut(greedy.S,greedy.Sb);
        }
        max2=max2/5;

            int v1 = maxCut.Vsize;
            int v2 = maxCut.Esize;
            int v3 = maxCut.calculateCut(randomized.S, randomized.Sb);
            int v4 = max2;
            int v5 = maxCut.calculateCut(semigreedy.S, semigreedy.Sb);
            int v6 = local.iteration;
            int v7 = maxCut.calculateCut(local.S, local.Sb);
            int v8 = it;
            int v9 = grasp;

            int max1 = 0;
            if (v3 > max1)
                max1 = v3;
            if (v4 > max1)
                max1 = v4;
            if (v5 > max1)
                max1 = v5;
            if (v7 > max1)
                max1 = v7;
            if (v9 > max1)
                max1 = v9;

            String content =input+" "+ String.valueOf(v1) + " " + String.valueOf(v2) + " " + String.valueOf(v3) + " " + String.valueOf(v4) + " " + String.valueOf(v5) + " " + String.valueOf(v6) +
                    " " + String.valueOf(v7) + " " + String.valueOf(v8) + " " + String.valueOf(v9) + " " + String.valueOf(max1);

            FileWriter fileWriter = new FileWriter(maxCut.file);
            BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
            bufferedWriter.write(content);
            bufferedWriter.close();
   }
}