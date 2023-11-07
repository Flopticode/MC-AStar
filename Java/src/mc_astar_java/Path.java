package mc_astar_java;

import java.util.Vector;

public class Path
{
	private Vector<Position> path;
	
	public Path(Vector<Position> path)
	{
		this.path = path;
	}
	
	public int getLength()
	{
		return path.size();
	}
	public Vector<Position> getPath()
	{
		return path;
	}
}
