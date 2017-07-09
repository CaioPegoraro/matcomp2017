import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
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

/** first index is always the source
* * last index is always the sink
* *
* *
* *
* */
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

    public ArrayList<Integer> findPath(int start) {
        ArrayList<Integer> path = new ArrayList<Integer>();
        if (start==numberNodes-1){
            path.add(start);
            return path;
        } else {
            ArrayList<Integer> returnedPath = null;
            for (int i = 0; i < numberNodes; i++) {
                int lFlow = nodes[start].costs[i];
                if (lFlow > 0) {
                    returnedPath = findPath(i);

                    if(returnedPath.size()>0 && !returnedPath.contains(start)){
                        path.add(start);
                        path.addAll(returnedPath);
                        return path;
                    }
                }
            }
            return path;
        }
    }

    public Integer getFlow(Integer source, Integer target) {
        return nodes[source].costs[target];
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

        Graph grafo  = new Graph(numberNodes);
        Graph residualGraph = new Graph(numberNodes);

        for (int i = 0; i<numberNodes;i++){
            Node node = new Node(numberNodes);
            Node residualNode = new Node(numberNodes);
            for (int j = 0; j<numberNodes;j++){
                int cost = in.nextInt();
                node.setNeighborsCost(j,cost);
                residualNode.setNeighborsCost(j,cost);
            }
            grafo.setNode(i,node);
            residualGraph.setNode(i,residualNode);
        }
        grafo.print();
        ArrayList<Integer> path = residualGraph.findPath(0);
        Integer minFlow = residualGraph.getFlow(path.get(0),path.get(1));
        for (int i = 1; i<path.size()-1;i++){
            minFlow=Math.min(minFlow,residualGraph.getFlow(path.get(i),path.get(i+1)));
        }

        System.out.println(path + "  "+ minFlow);

    }
}
