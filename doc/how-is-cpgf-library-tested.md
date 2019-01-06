

# How is cpgf library tested and quality assurance

## Several measures are adopted to test the library

  * Unit tests. Hundreds of unit tests were written and ran. More unit tests are being added for existing code and new code. The development is driven by unit tests.
  * Valgrind. valgrind is used to check memory leaks and other memory problems (wild pointer, double free, etc). Usually it's guaranteed there is no memory leak from cpgf library.
  * Code coverage. GNU gcov is used to check the code coverage in unit tests. The ultimate goal is to cover all API functions and 80% implementation code. Though we didn't achieve the goal yet, we are closing to it day by day.
  * Meta data and script binding for real third party libraries. This is not tests, but meta data is generated for several third party libraries and tested in script binding. This proves cpgf library can work well in the real world.

## You help is needed

If you use cpgf in your real project and it works for you, please let me know. The usage in real world projects is very important to approve the value of cpgf library.
