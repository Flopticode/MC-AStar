package mc_astar_java;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

public class MC_AStar_Java
{	
	public MC_AStar_Java()
	{
		try
		{
			loadLib("MC-AStar-V1-1.dll");
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
	
	private void loadLib(String libName)
	{
		File locallibfile = new File("libs/" + libName);
		if(locallibfile.exists())
		{
			System.load(locallibfile.getAbsolutePath());
			return;
		}
		
		File libfile = null;
		InputStream stream;
        try
        {
        	var tmpDirPath = Files.createTempDirectory("MC-AStar-libs");
    		var tmpDir = tmpDirPath.toFile();
    		tmpDir.deleteOnExit();

            libfile = new File(tmpDir.getPath(), libName);
        	
        	stream = MC_AStar_Java.class.getClassLoader().getResourceAsStream("libs/" + libName);
            
        	Files.copy(
            		stream,
            		libfile.toPath(),
            		StandardCopyOption.REPLACE_EXISTING);
            stream.close();
            
            System.load(libfile.getPath());
        } catch (IOException | NullPointerException e) {
        	if(libfile != null)
        		libfile.delete();
        	e.printStackTrace();
        }
        
        
	}
}
