import java.util.*;
class Example{
	public static void main(String args[]){
		Scanner input = new Scanner(System.in);

		Random r = new Random();
		int[][] marks = new int[10][4];
		for(int j = 0; j< 10; j++){
			//System.out.println("Input Marks for Student "+ (j+1));
			for(int i = 0; i< 4; i++){
				//System.out.print("Input Marks for Subject "+(i+1)+" : ");
				marks[j][i] = r.nextInt(101);
			}
		}
		// print marks

	}
}
