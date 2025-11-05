/**
* pizza.c
* CPSC 275
* Sean Balbale
**/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
  int brand;          // brand code
  int product;        // product code
  int store;          // store code
  int pizzas_per_package; // number of pizzas per package
  float diameter;     // diameter in inches
  float price;        // price in dollars
  float cost_per_sq_inch; // computed cost per square inch
} item;

void getinput(item *ip, int n);        // reads n items from stdin; returns number read
void computecost(item *ip, int n);   // computes cost per square inch for each item
void sortitems(item *ip, int n);     // sorts by non-decreasing cost per square inch
void displayitems(item *ip, int n);  // prints the sorted list

int main(){
  int n;
  scanf("%d", &n); // read number of items

  item *ip = (item *)malloc(n * sizeof(item)); // allocate memory for 100 items
  
  getinput(ip,n);

  computecost(ip,n);
  sortitems(ip,n);
  displayitems(ip, n);

  free(ip); // free allocated memory

  return 0;
}

void getinput(item *ip, int n){
  for (int i = 0; i < n; i++) {
    scanf("%d %d %f %d %d %f", &ip[i].brand, &ip[i].product, &ip[i].diameter, &ip[i].pizzas_per_package, &ip[i].store, &ip[i].price);
  }
}

void computecost(item *ip, int n){
  for (int i = 0; i < n; i++) {
    float radius = ip[i].diameter / 2.0;
    float area = ip[i].pizzas_per_package * M_PI * radius * radius;
    ip[i].cost_per_sq_inch = ip[i].price / area;
  }
}

void displayitems(item *ip, int n){
  char brands[6][20] = {"", "DiGiorno", "Red Baron", "Totino’s", "Kirkland", "Generic"};
  char products[5][20] = {"", "Cheese", "Pepperoni", "Supreme", "Veggie"};
  char stores[5][20] = {"", "Target", "Walmart", "Costco", "Big Y"};

  for (int i = 0; i < n; i++) {
    printf("%s %s at %s: %.5f\n", brands[ip[i].brand], products[ip[i].product], stores[ip[i].store], ip[i].cost_per_sq_inch);
  }
}

void sortitems(item *ip, int n){
  // Implement a bubble sort based on cost_per_sq_inch
  for (int i = 0; i < n-1; i++) { // Number of passes
    for (int j = 0; j < n-i-1; j++) { // Compare adjacent items
      if (ip[j].cost_per_sq_inch > ip[j+1].cost_per_sq_inch) { // If out of order
        // Swap:
        item temp = ip[j];
        ip[j] = ip[j+1];
        ip[j+1] = temp;
      }
    }
  }
}
