SAGAS.cpp
========

## Requirements
### C++ version
We confirmed that C++14(g++ Ubuntu 7.5.0-3ubuntu1~18.04) and C++17(g++ (Homebrew GCC 11.2.0_3) 11.2.0) successfully compiled SAGAS.cpp.

## How to run

### Google colaboratory (Easy, recommend)

1. Access this [page](https://colab.research.google.com/github/bioinfo-tsukuba/SAGAS/blob/main/SAGASforGithub.ipynb)
3. Follow the instructions in the notebook.


### Local environment

1. Get the source code.
    1. If you are familiar with `git`, simply clone this repository and move into the directory.
        ```zsh
        $ git clone https://github.com/bioinfo-tsukuba/SAGAS.git
        $ cd SAGAS
        $ pwd
        /path/to/SAGAS
        ```
    1. Alternatively,
        1. Download [a zip archive](https://github.com/bioinfo-tsukuba/SAGAS/archive/main.zip).
        2. Extract the archive (a directory named `SAGAS-main` will be created).
        3. `cd` into the directory.
2. Compile SAGAS.cpp
    ```zsh
    g++ SAGAS.cpp -o SAGAS_compiled
    ```
4. Run example cases.
    ```zsh
     $ ./SAGAS_compiled -log Sample "This is sample case" -conf ./sample/configs/Gu2016x1_sample.tsv
    ```
    
    <details><summary>Short explanation</summary>
    
    ```zsh
    SAGAS_compiled -log Userbane "memo" -conf config.tsv [-penalty {penalty}] [-gl {gl} {failedskip}]
    ```

    </details>
5. Make setting files(dependency.tsv, machines.tsv, operations.tsv, tcmb.tsv) in a directory. Blank templates are in the "Blank templates" directory.
    <details><summary>Detail explanation for making setting files</summary>
    
    1. machines.tsv
        
        The No.[Machine_ID] machine is a type [Machine_type] machine. It is called [Machine_name].
        
    1. operations.tsv
    
        The [Operation_ID]-th operation in the [Job_ID]-th job can be processed by the type [Compatible_machine] machine within [Processing_time] \(msec).
        
    1. dependency.tsv
        
        Let the [Operation_ID_1]-th operation in the [Job_ID]-th job be $O_a$.
    
        Let the [Operation_ID_2]-th operation in the [Job_ID]-th job be $O_b$.
    
        $O_a$ must finish before $O_b$ begins.
        
    1. tcmb.tsv (a little bit complex)
    
        Let the [Operation_ID_1]-th operation in the [Job_ID]-th job be $O_a$.
    
        Let the [Operation_ID_2]-th operation in the [Job_ID]-th job be $O_b$.
        
        1. Pattern A ([Point_1] == Start && [Point_2] == Start)
            
            The absolute difference between the start time of operation $O_a$ and the start time of operation $O_b$ must be less than or equal to [Time_constraint].
            
        1. Pattern A ([Point_1] == End && [Point_2] == Start)
        
            The absolute difference between the end time of operation $O_a$ and the start time of operation $O_b$ must be less than or equal to [Time_constraint].
                    
        1. Pattern A ([Point_1] == Start && [Point_2] == End)
        
            The absolute difference between the start time of operation $O_a$ and the end time of operation $O_b$ must be less than or equal to [Time_constraint].
                    
        1. Pattern A ([Point_1] == End && [Point_2] == End)
        
            The absolute difference between the end time of operation $O_a$ and the end time of operation $O_b$ must be less than or equal to [Time_constraint].
    
    </details>
    
6. Make a config file (you can use the interactive config maker).


## Output component

1. test_dep*.tsv: All dependencies for visualisation.
2. test_machines*.tsv: All machines for visualisation.
3. test_time*.tsv: All schedules (each start time and each allocated machine).
4. test_result*.tsv: Summary of each scheduling in an experiment includes the following:
    1. Date of experiment
    2. Experiment ID
    3. Number of units per type of instrument installed
    4. Number of units of each type of instrument actually used
    5. Score (execution time)
    6. The computation time of SA steps
    7. The computation time of the entire schedule
    8. The simulated-annealing step where the best schedule found
    9. The total number of simulated-annealing steps
    10. Evaluation of the output solution (Greedy-derived, SA-derived or failed)
5. \*\_log.txt: Log includes the following:
    1. All configurations for the simulation
    2. (optional) User name
    3. (optional) Comment
    4. (optional) Comment date



## (optional)

## References
