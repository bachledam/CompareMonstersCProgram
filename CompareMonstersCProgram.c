#include "monster-sorts.h"
#include "leak_detector_c.h"

void print_clocks(clock_t clocks) {
  printf("  %lfs CPU time used\n", ((double) clocks) / CLOCKS_PER_SEC);
}
void swap_monsters(monster *list, int i, int j)
{
    monster temp;

    memcpy(&temp, list + i, sizeof(monster));
    memcpy(list + i, list + j, sizeof(monster));
    memcpy(list + j, &temp, sizeof(monster));
    Swaps++;
}
/* The core comparison function. */

int compare_monsters(monster *m1, monster *m2, int use_name, int use_weight)
{
    Comparisons++;
  // Implement this function.
    if (use_name == 1){ //names
        return strcmp(m1->name, m2->name);
    }
    else { //weights
        if (m1->weight > m2->weight){ //m1 weight is greater
            return 1;
        }
        else if (m1->weight < m2->weight){ //m1 weight is smaller
            return -1;
        }
        else { //m1 and m2 are equal
            return 0;
        }
    }
}

/* Implement ascending quick sort. */
//
int repartition(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
    int i = low_index;
    int j = high_index;

    monster pivot = list[low_index]; //pivot starts at head


    while (i < j)
    {
        while (compare_monsters(&pivot, &list[i], use_name, use_weight) >= 0 && i<high_index){//increments i when weight is greater
            i++;
        }
        while (compare_monsters(&pivot, &list[j], use_name, use_weight) < 0){//decrements j when weight is smaller
            j--;
        }
        if (i < j){
            swap_monsters(list, i, j);
        }
    }
    swap_monsters(list, low_index, j);
    return j;
}

/* Recursive function for quick sort. */

void quick_sort_recursive(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
    if(low_index < high_index){
        int pivot_index = repartition(list, low_index, high_index, use_name, use_weight); //creates ints for recursive call and partitions the list
        quick_sort_recursive(list, low_index, pivot_index - 1, use_name, use_weight);
        quick_sort_recursive(list, pivot_index + 1, high_index, use_name, use_weight); //recursive call to continue from low to pivot-1 and then pivot+1 to high
    }
}

/* Shell function for quick sort. */

void quick_sort(monster *list, int n, int use_name, int use_weight)
{
    printf("Quick sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
    clear_stats();
    quick_sort_recursive(list, 0, n-1, use_name, use_weight);
    print_stats();
}

/* Implement ascending bubble sort. */

void bubble_sort(monster *list, int n, int use_name, int use_weight)
{
    int i; //creates ints for loops
    int j;
    printf("Bubble sort %d monsters...\n", n);
    clear_stats();
    for(i = n-1; i >= 0; i--) //starts at tail and decrements
    {
        for(j = 0; j < i; j++) //starts at head and increments
        {
            Comparisons++;
            if (compare_monsters(&list[j], &list[j + 1], use_name, use_weight) >= 0) //swaps when finds smallest elements
            {
                swap_monsters(list, j, j+1);
            }
        }
    }
    print_stats();
}
//* Highest-value finder for selection sort. */

int find_highest(monster *list, int n, int use_name, int use_weight)
{
  // Implement this function.
  int highest_loc = 0;
  int i;
  for(i = n; i > 0; i--)
  {
    Comparisons++;
    if((compare_monsters(&list[i], &list[highest_loc], use_name, use_weight) > 0)) //calls compare monster to find the highest location
    {
      highest_loc = i;
    }
  }
  return highest_loc;
}

///* Implement ascending selection sort. */
void selection_sort(monster *list, int n, int use_name, int use_weight)
{
  int highest;
  printf("Selection sort %d monsters...\n", n);
  clear_stats();
  // One by one move boundary of unsorted subarray
  for(int i = n-1; i > 0; i--)
  {
    // Find the maximum element in unsorted array
    highest = find_highest(list, i, use_name, use_weight);
    if(highest != i)
    {
      swap_monsters(list, highest, i); //Swap the found maximum element with the last element
    }
  }
  print_stats();
}
/* Find position for insertion sort. */

int insertion_sort_find_position(monster *list, int low_index, int high_index, monster *k, int use_name, int use_weight)
{
    int loc;
    for(int i = low_index; i < high_index; i++){
        loc = i;
        if(compare_monsters(k, &list[i], use_name, use_weight) > 0){ //calls compare monster to find the smallest monster
            return loc;
        }
    }
    return loc;
}

void insertion_sort_internal(monster *list, int n, int use_name, int use_weight)
{
    int j;
    monster selected;
    for (int i = 1; i < n; ++i) {
        selected = list[i];
        j = i - 1;
        int k = insertion_sort_find_position(list,0 ,i-1 ,list + 1 ,use_name, use_weight);
        if (k != i){
        // Compare key with each element on the left of it until an element smaller than it is found.
        // Move all elements after location to create space
        while (j >= 0 && compare_monsters(&list[j], &selected, use_name, use_weight) > 0) {
            list[j + 1] = list[j];
            j = j -1;
        }
        list[j+1] = selected;
        }
    }
}
/* Shell for insertion sort. */

void insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  printf("Insertion sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  clear_stats();

  insertion_sort_internal(list, n, use_name, use_weight);

  print_stats();
  return;
}

/* Merge function for merge sort.  Merges sub-arrays [l1..h1] and [l2..h2].
   Must have l2 = l1 + 1 or bad things will happen. */

void merge_sort_merge(monster *list, int l1, int h1, int l2, int h2, int use_name, int use_weight)
{
        int i,j,k;
        int size1 = h1 - l1 + 1;
        int size2 = h2 - l2 + 1;
        //create temp arrays
        monster *left = malloc(sizeof(monster) * size1);
        monster *right = malloc(sizeof(monster) * size2);

        //copy data to arrays
        for (i = 0; i < size1; i++){
            left[i] = list[l1 + i];
        }
        for (j = 0; j < size2; j++){
            right[j] = list[l2 + j];
        }
        //merge the temp arrays back
        i =0;
        j = 0;
        k = l1;
        while (i < size1 && j < size2){
            if (compare_monsters(&left[i], &right[j], use_name, use_weight) <= 0){
                list[k] = left[i];
                i++;
            }
            else {
                list[k] = right[j];
                j++;
            }
            k++;
        }

        while (i < size1) {
            list[k] = left[i];
            i++;
            k++;
        }

        /* Copy the remaining elements of right[], if there are any */
        while (j < size2) {
            list[k] = right[j];
            j++;
            k++;
        }
        free(left);
        free(right);
}

void merge_sort_recursive(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
    int b = high_index - low_index + 1;
    if (low_index < high_index) {
        int m =b/2;
        if (b <= 1){
            return;
        }
        // Sort first and second halves
        merge_sort_recursive(list, low_index ,m + low_index-1, use_name, use_weight);
        merge_sort_recursive(list, m+low_index, high_index, use_name, use_weight);
        merge_sort_merge(list, low_index, low_index + m-1, m+low_index, high_index, use_name,use_weight);
    }
}

/* Implement merge sort. */

void merge_sort(monster *list, int n, int use_name, int use_weight)
{
  printf("Merge sort %d monsters...\n", n);

  clear_stats();
  merge_sort_recursive(list, 0, n-1, use_name, use_weight);
  print_stats();
}

/* Recursive function for merge-insertion sort. */

void merge_insertion_sort_recursive(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
        int b = high_index - low_index + 1;
        int m = b/2;

        if (b <= 1){
            return;
        }
        if (b <= 25){
            insertion_sort_internal(&list[low_index], b, use_name, use_weight);
            return;
        }
        merge_insertion_sort_recursive(list, low_index ,low_index + m - 1, use_name, use_weight);
        merge_insertion_sort_recursive(list, low_index + m, high_index, use_name, use_weight);
        merge_sort_merge(list, low_index, low_index + m - 1, low_index + m, high_index, use_name,use_weight);
}
void merge_insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  clear_stats();
  printf("Merge-insertion sort %d monsters...\n", n);

  merge_insertion_sort_recursive(list, 0, n-1, use_name, use_weight);

  print_stats();
}
