#ifndef QUICK_SORT_H
#define QUICK_SORT_H

int Compare(const void *x, const void *y);

void QuickSort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *));

#endif
