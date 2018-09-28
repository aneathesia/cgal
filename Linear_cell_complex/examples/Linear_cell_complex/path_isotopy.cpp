#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Combinatorial_map_functionalities.h>

/* If you want to use a viewer, you can use qglviewer. */
#ifdef CGAL_USE_BASIC_VIEWER
#include <CGAL/LCC_with_paths.h>
#endif

#include <CGAL/Path_generators.h>
#include <CGAL/Path_on_surface.h>

typedef CGAL::Linear_cell_complex_for_combinatorial_map<2,3> LCC_3_cmap;
///////////////////////////////////////////////////////////////////////////////
void usage(int /*argc*/, char** argv)
{
  std::cout<<"usage: "<<argv[0]<<" file [-draw] [-seed S] [-nbfaces F] [-nbdefo D]"<<std::endl
           <<"   Loadd the given off file, compute one random path, deform it into a second path and "
           <<"   test that the two pathes are isotopic."<<std::endl
           <<"   -draw: draw mesh and the two pathes." <<std::endl
           <<"   -seed S: uses S as seed of random generator. Otherwise use a different seed at each run (based on time)."<<std::endl
           <<"   -nbfaces F: use F connected random faces to generate the initial path (by default a random number beween 10 and 100)."<<std::endl
           <<"   -nbdefo D: use D deformations to generate the second path (by default a random number between 10 and 100)."<<std::endl
           <<std::endl;
  exit(EXIT_FAILURE);
}
///////////////////////////////////////////////////////////////////////////////
void error_command_line(int argc, char** argv, const char* msg)
{
  std::cout<<"ERROR: "<<msg<<std::endl;
  usage(argc, argv);
}
///////////////////////////////////////////////////////////////////////////////
void process_command_line(int argc, char** argv,
                          std::string& file,
                          bool& draw,
                          unsigned int& F,
                          unsigned int& D,
                          CGAL::Random& random)
{
  std::string arg;
  for (unsigned int i=1; i<(unsigned int)argc; ++i)
  {
    arg=argv[i];
    if (arg=="-draw")
    { draw=true; }
    else if (arg=="-seed")
    {
      if (i==argc-1)
      { error_command_line(argc, argv, "Error: no number after -seed option."); }
      random=CGAL::Random(std::stoi(std::string(argv[++i]))); // initialize the random generator with the given seed
    }
    else if (arg=="-nbfaces")
    {
     if (i==argc-1)
     { error_command_line(argc, argv, "Error: no number after -nbfaces option."); }
     F=std::stoi(std::string(argv[++i]));
    }
    else if (arg=="-nbdefo")
    {
     if (i==argc-1)
     { error_command_line(argc, argv, "Error: no number after -nbdefo option."); }
     D=std::stoi(std::string(argv[++i]));
    }
    else if (arg=="-h" || arg=="--help" || arg=="-?")
    { usage(argc, argv); }
    else if (arg[0]=='-')
    { std::cout<<"Unknown option "<<arg<<", ignored."<<std::endl; }
    else
    { file=arg; }
  }

  if (F==0)
  { F=random.get_int(10, 101); }

  if (D==0)
  { D=random.get_int(10, 101); }
  
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  std::string file="data/3torus-smooth.off";  
  bool draw=false;
  unsigned int F=0;
  unsigned int D=0;
  CGAL::Random random; // Used when user do not provide its own seed.

  process_command_line(argc, argv, file, draw, F, D, random);  

  LCC_3_cmap lcc;
  if (!CGAL::load_off(lcc, file.c_str()))
  {
    std::cout<<"PROBLEM reading file "<<file<<std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout<<"Random seed: "<<random.get_seed()<<std::endl;
  std::cout<<"Initial map: ";
  lcc.display_characteristics(std::cout) << ", valid="
                                         << lcc.is_valid() << std::endl;
  
  CGAL::Combinatorial_map_tools<LCC_3_cmap> cmt(lcc);
  std::cout<<"Reduced map: ";
  cmt.get_map().display_characteristics(std::cout)
    << ", valid="<< cmt.get_map().is_valid() << std::endl;
  
  std::vector<const CGAL::Path_on_surface<LCC_3_cmap>* > paths;
  std::vector<CGAL::Path_on_surface<LCC_3_cmap> > transformed_paths;
  
  CGAL::Path_on_surface<LCC_3_cmap> path1(lcc);
  generate_random_closed_path(path1, F, random); // random path, length between 30 and 500
  std::cout<<"Path1 size: "<<path1.length()<<" (from "<<F<<" faces); ";
  paths.push_back(&path1);

  CGAL::Path_on_surface<LCC_3_cmap> path2(path1);
  update_path_randomly(path2, D, random);
  std::cout<<"Path2 size: "<<path2.length()<<" (from "<<D<<" deformations): ";
  paths.push_back(&path2);
  std::cout<<std::flush;
  
  CGAL::Path_on_surface<LCC_3_cmap> transformed_path1=
    cmt.transform_original_path_into_quad_surface(path1);

  CGAL::Path_on_surface<LCC_3_cmap> transformed_path2=
    cmt.transform_original_path_into_quad_surface(path1);
  
  transformed_path1.canonize();
  transformed_path2.canonize();
    
  if (transformed_path1!=transformed_path2)
  {
    std::cout<<"ERROR: pathes are not isotopic while it should be. (";
    transformed_path1.display();
    std::cout<<") != (";
    transformed_path2.display();
    std::cout<<")"<<std::endl;
  }
  else
  { std::cout<<"TEST OK: pathes are isotopic."<<std::endl; }
  
  if (draw)
  { display(lcc, paths); }


  return EXIT_SUCCESS;
}
