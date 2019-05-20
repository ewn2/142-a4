# 142-a4
CMPE 142 - Operating Systems Assignment 4: Concurrency

Producer-Consumer Problem that deals with 4 threads and uses
Semaphores to prevent deadlocks

Scenario is a chef producing 2 of 3 parts of a meal at every round,
then napping until a customer rings the counter bell,
3 Customers, each with an infinite supply of one of the 3 parts of
the full meal eat then ring the bell to wake up the chef.
After 100 servings out of the chef, the kitchen closes for the day
and the total amount of meals eaten by each customer is tallied.


Chef produces two random meal components (out of Hamburger, Fries, or Soda) at a time, then goes to sleep until a customer
tells him to make something else

Customer #1 brings their own infinite supply of hamburgers, and are only waiting for fries and soda.
Customer #2 brings their own infinite supply of fries, and are only waiting for hamburgers and soda.
Customer #3 brings their own infinite supply of soda, and are only waiting for hamburgers and fries.
