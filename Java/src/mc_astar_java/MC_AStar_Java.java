package mc_astar_java;

public class MC_AStar_Java
{	
	public MC_AStar_Java()
	{
		var osName = System.getProperty("os.name"); 
		if(!osName.equals("Windows 10"))
			throw new UnsupportedOperationException("Currently, only Windows 10 is supported by MC-AStar. You are using " + osName + ".");
			
		try
		{
			System.loadLibrary("libs/MC-AStar-V1");
			System.out.println("Library loaded.");
		}
		catch(UnsatisfiedLinkError e)
		{
			throw new RuntimeException("Loading the MC-AStar native library failed.", e);
		}
		
		var err = init();
		if(err != MCAStarError.MC_ASTAR_NO_ERROR)
		{
			throw new RuntimeException("Initialization failed with code " + err + "!");
		}
	}
	
	public native int[] findPath(int startX, int startY, int startZ, int endX, int endY, int endZ);
	public native MCAStarError init();
	public native MCAStarError addChunk(int chunkX, int chunkY, int chunkZ, long[] chunk);
	public native MCAStarError setBlock(int x, int y, int z, long state);
}
