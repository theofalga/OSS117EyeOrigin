import ij.*;
import ij.gui.*;
import java.awt.*;
import ij.plugin.filter.PlugInFilter;
import ij.process.*;
import java.awt.event.*;
import java.io.*;
//vector
import java.util.*;
//writer lib
import java.lang.Object.*;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.File;

/*
Authors: TE Christian, HUBERT Alexis, FALGARONE Théo, MAUNIER Tristan
Version: 3.0
Date: 26 april 2017
Project: OSS 117: Eye Origin

ImageJ plugin to get reference points from image library. 

// Instructions:

// Creer le stack dans imagej :
// File -> import -> image sequence -> sélectioner n'importe quel image du fichier -> OK
// La génération du sha ne marche pas si on ouvre un stack déjà créé (problème de chemin et de nom à cause de slices/stack) /!\

// Lancer le plugin:
// Plugins --> Compile and run --> Oss117plugin.java
// Clique gauche  sur l'iris gauche, puis sur l'iris droit. En cas de problème sur une image clique roulette pour retourner à l'image précédente.


//To create a stack in imageJ:
// File -> import -> image sequence -> select one of the image -> tick: use virtual stack -> OK
//Don't use already made stack, or conflic between path and slide name will occur.

// Plugin run and use:
// Plugins --> Compile and run --> Oss117plugin.java
// Left click on the left iris, then on the right one. If you did a mistake on an slide, keep going to the next, then you can go on the previous one by clicking on the midle button of the mouse. 
*/

public class Oss117plugin implements PlugInFilter, MouseListener
{
    ImagePlus img;
    ImageCanvas canvas;
    int cpt = 0;
    int frame = 1;
    String path;
    String fichier;
    String sha = "sha1sum ";
    String extension = ".png";
    String filename = "positionReference.txt";
    String entete = "Left_X  Left_Y  Right_X  Right_Y \n";
   
    public int setup(String arg, ImagePlus img)
    {
	this.img = img;
	IJ.register(Oss117plugin.class);
	return DOES_ALL+NO_CHANGES;	
    }
    public void run(ImageProcessor ip)
    {
	this.path=  img.getOriginalFileInfo().directory;
	this.fichier = path + filename;
	File f = new File (fichier);

	try
	    {	    
		FileWriter fw = new FileWriter (f);
		fw.write (entete);
		fw.close();
	    }
	catch (IOException exception)
	    {System.out.println ("Erreur de création du fichier : " + exception.getMessage());}
	
	ImageWindow win = img.getWindow();
	canvas = win.getCanvas();
	canvas.addMouseListener(this);
	
    }

    //function that write x and y position of the click into the file
    public void save(File f, int X, int Y){
	try{
	    FileWriter fw = new FileWriter (f,true);
	    fw.write (String.valueOf (X));
	    fw.write (" "+String.valueOf (Y)+" ");
	    fw.close();
	}
	catch (IOException exception)
	    {System.out.println ("Erreur lors de la sauvegarde : " + exception.getMessage());}
    }

    //each click
    public void mousePressed(MouseEvent e){
	cpt++;
	int X = e.getX();
	int Y = e.getY();
	int button = e.getButton();
	
	File f = new File (fichier);
	
	//if (frame == nbslice+1){img.close();} try to get the number of slice in the stack to close it at the end
	
	//left click
	if (button == MouseEvent.BUTTON1) {
	    //first on the slide (left iris)
	    if (cpt == 1){
		try
		    {	    
			FileWriter fw = new FileWriter (f,true);
			String s = null;
		       	String name= img.getImageStack().getShortSliceLabel(img.getCurrentSlice());
			char repere = name.charAt(3);
			int rep = Character.getNumericValue(repere);
			if (rep == 1) //write cam position and angle only once for 9 dotes
			    {
				char position = name.charAt(0);
				char hangle = name.charAt(1);
				char vangle = name.charAt(2);	
				fw.write(position+" ");
				fw.write(hangle+" ");
				fw.write(vangle+"\n");
			    }
			
			fw.write(repere+"\n");
			Process p = Runtime.getRuntime().exec(sha+path+name+extension);
			BufferedReader stdInput = new BufferedReader(new InputStreamReader(p.getInputStream()));		       
			while ((s = stdInput.readLine()) != null) {
			   fw.write(s+"\n");
			   IJ.log(s);
			   }
			fw.close();
			save(f,X,Y);
		    }
		catch (IOException exception)
		    {System.out.println ("Left iris capture error: " + exception.getMessage());}
	    }
	    //second on the slide (right iris)
	    if (cpt == 2)
		{
		try{	    
			FileWriter fw = new FileWriter (f,true);
			save(f,X,Y);
			fw.write ("\n");
			fw.close();		   
		    }
		catch (IOException exception)
		    {System.out.println ("right iris capture error: " + exception.getMessage());}
		
		IJ.run(img, "Next Slice [>]", "");
		frame++;
		cpt = 0;
		}
	    IJ.log("x: "+X+"  y: "+Y);
	    
	}
	//if mistaked one slide, can reverse by pressing middle button (can be changed right3)
	if ( button == MouseEvent.BUTTON2) 
	    {
		IJ.run(img,"Previous Slice [<]", " ");
		frame--;
		cpt=0;
		deleteLine(fichier);
	    }
    }

    //this function read all the file and rewrite it without the last line
    public boolean deleteLine(String fileName) {
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(fileName)));

	    ArrayList <String> lines = new ArrayList<String>();
	    BufferedReader old = new BufferedReader(new FileReader(fileName));
	    String in;
	    while ((in = old.readLine()) != null)
		lines.add(in);
	    old.close();
	    
	    lines.remove(lines.size() - 3);
	    lines.remove(lines.size() - 2);
	    lines.remove(lines.size() - 1);

	    PrintWriter newfile = new PrintWriter(new FileWriter(fileName));
	    for (String line : lines)
		newfile.println(line);
	    newfile.close();
 
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    public void mouseReleased(MouseEvent e){}
    public void mouseExited(MouseEvent e){}  
    public void mouseClicked(MouseEvent e){}	
    public void mouseEntered(MouseEvent e){}

}
