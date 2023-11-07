package test;

import java.util.Random;

import mc_astar_java.MCAStarError;
import mc_astar_java.MC_AStar_Java;

public class PathFindingTest
{
	private static Random rand = new Random(System.currentTimeMillis());
	
	public static long[] generateRandomData()
	{
		long[] data = new long[16*16*16];
		for(int x = 0; x < 16; x++)
			for(int y = 0; y < 16; y++)
				for(int z = 0; z < 16; z++)
				{
					int breakDelay = ((rand.nextInt() % 10) + 1) & 0xFF;
					
					int index = z * 16 * 16 + y * 16 + x; 
					data[index] = (1 << 0) /* walk on */
							| (breakDelay << 1) /* break delay */
							| (1 << 9) /* walk through */
							| (0 << 10); /* invalid */
				}
		return data;
	}
	
	public static void main(String[] args)
	{
		var lib = new MC_AStar_Java();
		
		for(int i = 0; i < 32; i++)
		{
			var error = lib.addChunk(i, 0, 0, generateRandomData());
			if(error != MCAStarError.MC_ASTAR_NO_ERROR)
			{
				System.err.println("Adding chunk failed: " + error);
				return;
			}
			else
			{
				System.out.println("Chunk added successfully.");
			}
		}
		
		int[] path = null;
		long ts1 = System.currentTimeMillis();
		for(int x = 0; x < 3; x++)
		{
			if(x % 1 == 0)
				System.out.println(x);
			path = lib.findPath(0, 0, 0, 510, 1, 2);
		}
		long ts2 = System.currentTimeMillis();
		System.out.println("The process took " + (ts2-ts1) + " millis.");
		
		if(path == null)
			System.out.println("Pathfinding finished successfully. No path was found.");
		else
			System.out.println("Pathfinding finished successfully. A path with size "
		+ (path.length / 3) + " was found.");
	}
}
