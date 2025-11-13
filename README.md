To buid and run cmsis_nn_example with gcc then generate gcda and html
```
make cmsis_nn_example
make coverage-report 
make html 
open coverage_html/index.html
```

To buid and run cmsis_nn_example with ARM-CC then generate gcda and html
```
make cmsis_nn_example_arm
make run-cmsis_nn_arm
make coverage-report 
make html 
open coverage_html/index.html
```


Working build without coverage
```
make -f makefile.new all   
```