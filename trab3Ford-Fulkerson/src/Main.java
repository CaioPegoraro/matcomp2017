import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;

class Node{

    int numberNeighbors;
    int[] costs;

    Node(int neighbors){
        this.numberNeighbors = neighbors;
        costs = new int[neighbors];
    }
    void setNeighborsCost(int index, int cost){
        costs[index]=cost;
    }

    @Override
    public String toString() {
        String string = new String("");
        for (int i = 0; i<numberNeighbors;i++){
            if (costs[i]>9) {
                string = string.concat(costs[i] + " ");
            } else{
                string = string.concat(" " +costs[i] + " ");
            }
        }
        return string;
    }
}

class Graph{

    int numberNodes;
    Node[] nodes;

    Graph(int numberNodes){
        this.numberNodes = numberNodes;
        this.nodes = new Node[numberNodes];
    }

    void setNode(int index,Node node){
        nodes[index]=node;
    }

    public void print() {
        for (int i = 0; i<numberNodes;i++){
            System.out.println(nodes[i]);
        }
    }
}

public class Main {

    public static void main(String[] args) {

        Scanner in;
        if (args.length>0){
            try {
                in = new Scanner(new FileReader(args[0]));
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                in = new Scanner(System.in);
            }
        } else {
            in = new Scanner(System.in);
        }
        System.out.print("Number of nodes: ");
        int numberNodes = in.nextInt();
        System.out.println(numberNodes);

        Graph grafo = new Graph(numberNodes);

        for (int i = 0; i<numberNodes;i++){
            Node node = new Node(numberNodes);
            for (int j = 0; j<numberNodes;j++){
                node.setNeighborsCost(j,in.nextInt());
            }
            grafo.setNode(i,node);
        }

        grafo.print();

    }
}
