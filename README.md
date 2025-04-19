# CS300_Portfolio_Piece

  -What was the problem you were solving in the projects for this course?

The code in this project was designed for the computer science department at the fictitious company ABCU.  The code loads information about the universities courses and validates that the courses have a valid format.  Courses are then either individually printable or printed in alpha-numeric order.

  -How did you approach the problem? Consider why data structures are important to understand.

I approached this problem by analyzing the pros and cons of each data structure.  Based on the specifications, the data structure must be able to maintain total ordering, and have fast search and delete times.  I chose a binary search tree for its fast operations while maintaining total ordering.  Choosing the right data structure improves the speed and memory efficiency of a program.

  -How did you overcome any roadblocks you encountered while going through the activities or project?

The largest problem I faced in this project was an assignment restriction of all my code having to be in one file.  This created a linker error with the projects static member variables.  I overcame this problem by doing research through forums and finding that static member variables need to be instantiated outside of the class in the cpp file.

  -How has your work on this project expanded your approach to designing software and developing programs?

I walked into this class with a strong foundation in data structures from my previous school.  On this project I focused on using design patterns, OOP principles, and SOLID principles to make code easier to develop, more modular, maintainable, and extensible.

  -How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

This project gave me great practice in using inheritance in making code that is more maintainable, readable, and adaptable.  This can be seen in the AbstractOption class, which manages menu behaviors and is easy to add new menu options/ behaviors.  One way this program could become more reusable is to refactor the BSTree and Node class to use generics and pass the BSTree to the menu by reference rather than using a singleton.
