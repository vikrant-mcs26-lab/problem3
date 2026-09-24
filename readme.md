

## Execution
If running on unix like operating system, run the following, from the root directory (i.e. same directory as this file)

```bash
python ./python/automation.py --executable_dir ./build --resources ./resources --seed 7 --num_nodes 20 --step_size 10
```

If you are running the command from windows host, please run these commands instead in cmd (and not in powershell)
```bash
python  .\python\automation.py --executable_dir .\build --resources .\resources --seed 7 --num_nodes 20 --step_size 10 --windows
```

Output would be available in 
- `./resources/graph` directory: All the randomly generated graphs.
- `./resources/vertex` directory: Vertex cover of corresponding Graphs.
- `./resources/approx` directory: Approximation of vertex cover along with accounting information
- `./resources/images` directory: The image file representing Optimal Vertex Cover, Maximal Matching, and Approximated Vertex Cover. 
- `./resources/table_data.csv` : Accounting data from various runs along with additional information in tabular format. 

# Setting Up
## Dependencies
- CMake (Version 3.16+)
- GNU C Compiler (C++ Std 17)
- Python (Version 3.9+)


## C++ Project
Run these commands in root directory of project (i.e. problem1) to compile c++ project.
```bash
mkdir build
cd ./build
cmake ..
cmake --build .
```

Run following commands in case of Windows Host for compiling
```bash
mkdir build
cd ./build
cmake .. -G "MinGW Makefiles"
cmake --build .
```


## Python Project
In the root directory of project (i.e. problem1), run following commands to setup python 
```bash
python -m venv venv
source venv/bin/activate
pip install -r ./python/requirement.txt
```

Run these commands instead if you are running it on windows host. 
```bash
python -m venv venv
.\venv\Scripts\activate
pip install -r ./python/requirement.txt
```