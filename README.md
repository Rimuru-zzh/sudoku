# sudoku

### 功能介绍：

该数独游戏程序支持生成数独终局、解决数独问题和生成数独游戏三个功能。以下是关于如何使用该程序的详细说明：

1. 生成数独终局

    命令格式：sudoku.exe -c 数量

    说明：该命令将生成指定数量的数独终局，并将结果写入文件"sudoku_syukyoku.txt"。生成的数独终局用于后续生成数独游戏。

    参数说明：
    * 数量：生成的数独终局的数量，范围为1到1000000。

2. 解决数独问题

    命令格式：sudoku.exe -s 数独文件路径

    说明：该命令将从指定的数独文件中读取数独问题，并解决该问题，将结果写入"output.txt"。

    参数说明：

    * 数独文件路径：包含数独问题的文件路径。

    * 解决方案文件路径：将解决方案写入的文件路径。

3. 生成数独游戏

    命令格式：sudoku.exe -n 数量 [可选项]

    说明：该命令将生成指定数量的数独游戏，并将结果写入文件"game.txt"。
    参数说明：

    * 数量：生成的数独游戏的数量，范围为1到10000。

    * 可选项：
    -u：每个生成的数独游戏必须具有唯一解。

    -r 范围：指定挖空的范围，即每个游戏中空格的数量范围。范围格式为"最小值~最大值"，例如"30~40"。默认范围为20~55。


### 注意事项：

* 生成数独终局和生成数独游戏时，程序将读取固定的数独终局文件"sudoku_syukyoku.txt"。确保该文件存在并包含数独终局。
* 解决数独问题时，程序将读取指定的数独问题文件，并将解决方案写入固定的文件。
* 程序将在生成数独游戏时使用生成的数独终局，并根据-r参数进行挖空生成游戏。


### 示例用法：

1. 生成100个数独终局：

    sudoku.exe -c 100

    结果将写入文件"sudoku_syukyoku.txt"。

2. 生成10个数独游戏，每个游戏具有唯一解，并且每个游戏中空格的数量范围为30~40：

    sudoku.exe -n 10 -u -r 30~40

    结果将写入文件"game.txt"。

3. 解决数独问题：

    sudoku.exe -s game.txt

    程序将从生成的game.txt文件或其他自定义的文件读取数独问题，解决问题，并将解决方案写入文件"output.txt"。


请注意，程序对多个文件进行的了读写。在使用程序时，请确保文件存在并提供正确的文件路径。
