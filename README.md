SAGAS.cpp
========

## Requirements
### C++ version
We confirmed that C++14(g++ Ubuntu 7.5.0-3ubuntu1~18.04) and C++17(GCC 9.2.1, clang-1316.0.21.2.5) successfully compiled SAGAS.cpp.


## How to build

```zsh
g++ SAGAS.cpp -o SAGAS_compiled
```

## How to run

### Google colaboratory (Easy, recommend)

1. Access this [page](https://github.com/bioinfo-tsukuba/SAGAS/blob/main/SAGASforGithub.ipynb)
2. Click this mark ![image](https://user-images.githubusercontent.com/85389241/197720096-68863e1b-a46d-4df3-98af-5ebf1a506812.png)
3. Follow the instructions in the notebook.


### Local environment

1. Get the source code.
    1. If you are familiar with `git`, simply clone this repository and move into the directory.
        ```sh
        $ git clone https://github.com/bioinfo-tsukuba/SAGAS.git
        $ cd SAGAS
        $ pwd
        /path/to/SAGAS
        ```
    1. Alternatively,
        1. Download [a zip archive](https://github.com/bioinfo-tsukuba/SAGAS/archive/main.zip).
        2. Extract the archive (a directory named `SAGAS-main` will be created).
        3. `cd` into the directory.


2. Run example cases.
    ```sh
     $ ./SAGAS_compiled -log Sample "This is sample case" -conf ./sample/configs/Gu2016x1_sample.tsv
    ```
3. Make setting files(dependency.tsv, machines.tsv, operations.tsv, tcmb.tsv) in a directory (see the sample files).//フォーマットを作ってあげるといいかも
4. Make a config file (see the sample file).//フォーマットを作ってあげるといいかも


```zsh
SAGAS_compiled -log Userbane "memo" -conf config.tsv [-penalty {penalty}] [-gl {gl} {failedskip}]'//後程確認
```





## (optional)

## References
