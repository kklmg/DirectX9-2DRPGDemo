#pragma once
//-------------------------------------------
//My Head File 
//-------------------------------------------
#include"queue.h"
#include"Heap.h"


//-------------------------------------------
//Swap 
//-------------------------------------------
template<typename T>
void __Swap(T arr[], int index_A, int index_B)
{
	T temp = arr[index_A];

	arr[index_A] = arr[index_B];

	arr[index_B] = temp;
}


//-------------------------------------------
//Bubble Sort
//-------------------------------------------
template<typename T>
void Sort_Bubble(T arr[],int length)
{
	int i;
	int j;

	for (i = 0; i < length -1; ++i)
	{
		for (j = 0; j < length - 1 - i; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				T temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			else continue;
		}

	}
}

template<typename T>
void Sort_Bubble(T[], int length, bool(*fptrCompare)(T, T))
{
	int i;
	int j;

	for (i = 0; i < length - 1; ++i)
	{
		for (j = 0; j < length - 1 - i; ++j)
		{

			if (fptrCompare(arr[j],arr[j + 1]))
			{			
				T temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;	
			}
			else continue;
		}
	}
}


//-------------------------------------------
//Slection Sort
//-------------------------------------------
template<typename T>
void Sort_Slection(T arr[], int length)
{
	int i;
	int j;

	int ChangeIndex;

	for (i = 0; i < length - 1; ++i)
	{
		ChangeIndex = i;

		for (j = i+1; j < length ; ++j)
		{
			if (arr[j] < arr[ChangeIndex])
			{
				ChangeIndex = j ;
			}
		}

		if (ChangeIndex == i)continue;

		T temp = arr[i];

		arr[i] = arr[ChangeIndex];

		arr[ChangeIndex] = temp;

	}
}


template<typename T>
void Sort_Slection(T arr[], int length ,bool(*fptrCompare)(T, T))
{
	int i;
	int j;

	int ChangeIndex;

	for (i = 0; i < length - 1; ++i)
	{
		ChangeIndex = i;

		for (j = i + 1; j < length; ++j)
		{
			if (fptrCompare(arr[j],arr[ChangeIndex]))
			{
				ChangeIndex = j;
			}
		}

		if (ChangeIndex == i)continue;

		T temp = arr[i];

		arr[i] = arr[ChangeIndex];

		arr[ChangeIndex] = temp;
	}
}


//-------------------------------------------
//Insertion Sort
//-------------------------------------------
template<typename T>
void Sort_Insertion(T arr[], int length) 
{
	int i, j;

	for (i = 1; i < length ; ++i)
	{

		T IsertData = arr[i];

		for (j = i; j > 0; --j)
		{
			if (IsertData < arr[j - 1])
			{
				arr[j] = arr[j-1];
			}
			else break;
		}
		arr[j] = IsertData;
	}
}

//-------------------------------------------
//Merge Sort
//-------------------------------------------
template<typename T>
struct STMergeSort
{
	void DivideSort(T arr[],int left,int right) 
	{
		if (left < right)
		{
			int mid = (right + left) / 2;

			DivideSort(arr, left, mid);

			DivideSort(arr, mid + 1, right);

			Merge(arr,left, right,mid);

		}
	}
	
	void Merge(T arr[], int left, int right,int mid) 
	{
		int Lindex = left;
		int Rindex = mid + 1;

		int InsertIndex = left;

		T *SortedArr = new T[right+1];

		while (Lindex <= mid && Rindex <= right)
		{
			if (arr[Lindex] <= arr[Rindex])
			{
				SortedArr[InsertIndex] = arr[Lindex];

				++Lindex;
			}
			else
			{
				SortedArr[InsertIndex] = arr[Rindex];

				++Rindex;
			}
			++InsertIndex;
		}


		if (Lindex > mid)
		{
			while (Rindex <= right)
			{
				SortedArr[InsertIndex] = arr[Rindex];

				++Rindex;
				++InsertIndex;
			}
		}
		else
		{
			while (Lindex <= mid)
			{
				SortedArr[InsertIndex] = arr[Lindex];

				++Lindex;
				++InsertIndex;
			}

		}

	
		for (int i = left; i <= right; ++i)
		{
			arr[i] = SortedArr[i];
		}
		
		delete[] SortedArr;
	}

	void operator()(T arr[], int left,int right) 
	{
		DivideSort(arr, left, right);
	}
};


//-------------------------------------------
//Quick Sort
//-------------------------------------------
template<typename T>
struct STQuickSort
{
	void InitPivot(T arr[], int left, int right )
	{
		if ((right-left) > 3) 
		{
			int pivotIndex = GetMidianOfThree(arr,left,left+1,left+2);

			__Swap<T>(arr, arr[left], arr[pivotIndex]);
		}
	}

	int GetMidianOfThree(T arr[],int index_A,int Index_B,int Index_C)
	{
		int tempARR[3] = { index_A,Index_B,Index_C };

		if (arr[tempARR[0]] > arr[tempARR[1]])
		{
			__Swap<int>(tempARR, 0, 1);
		}

		if (arr[tempARR[1]] > arr[tempARR[2]])
		{
			__Swap<int>(tempARR, 1, 2);
		}
		if (arr[tempARR[0]] > arr[tempARR[1]])
		{
			__Swap<int>(tempARR, 0, 1);
		}

		return tempARR[1];
	}

	void __QuickSort(T arr[], int left, int right)
	{
		if (left <= right)
		{
			int pivotIndex=Partition(arr, left, right);

			__QuickSort(arr, left, pivotIndex - 1);

			__QuickSort(arr, pivotIndex + 1, right);

		}
	}

	int Partition(T arr[], int left, int right)
	{
		int pivot = arr[left];

		int LowIndex = left+1;

		int HighIndex = right;

		while (HighIndex>= LowIndex)
		{
			while (pivot>=arr[LowIndex]&&LowIndex<=right)
			{
				++LowIndex;
			}
			
			while (pivot<=arr[HighIndex]&& HighIndex>=(left+1))
			{
				--HighIndex;
			}

			if (LowIndex <= HighIndex) 
			{
				__Swap<T>(arr,LowIndex, HighIndex);
			}
		}

		__Swap<T>(arr,left, HighIndex);//move pivot to highindex

		return HighIndex;//pivot index
	}

	void operator()(T arr[], int left,int right) 
	{
		InitPivot(arr, left, right);

		__QuickSort(arr, left, right);
	}
};


//-------------------------------------------
//Radix Sort
//-------------------------------------------






//void Sort_Radix(DWORD arr[],DWORD num, DWORD digitLength) 
//{
//	int radix;
//
//	int divfac = 1;
//
//	CArrQueue<DWORD> *queue=new CArrQueue<DWORD>[digitLength](num);
//
//	for (int pos = 0; pos < digitLength; ++pos) 
//	{
//		for (int di = 0; di < num; di++) 
//		{
//			radix == (arr[di] / divfac) % 10;
//		
//			queue
//		
//		
//		}
//	
//	
//	
//	}
//
//
//
//}
//
