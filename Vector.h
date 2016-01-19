/*
 * Vector.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sabeyruw
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * This is a basic vector class to support multiple architectures.
 */
template<typename T>
class Vector
{
  public:
    typedef T* iterator;
    typedef const T* const_iterator;

  private:
    int theSize;
    int theCapacity;
    T* objects;

  public:
    explicit Vector() : theSize(0), theCapacity(0), objects(0) {}

    Vector(const Vector& that) :
        objects(0)
    {
      operator=(that);
    }

    virtual ~Vector()
    {
      clear();
    }

    Vector& operator=(const Vector& that)
    {
      if (this != &that)
      {
        if (objects)
          free(objects);
        objects = 0;
        theSize = that.size();
        theCapacity = that.theCapacity;

        objects = (T*) malloc(capacity() * sizeof(T));
        for (int k = 0; k < size(); k++)
          objects[k] = that.objects[k];

      }

      return *this;
    }

    void resize(int newSize)
    {
      if (newSize > theCapacity)
        reserve(newSize * 2 + 1);
      theSize = newSize;
    }

    void reserve(int newCapacity)
    {
      if (newCapacity < theSize)
        return;

      T *oldArray = objects;

      objects = (T*) malloc(newCapacity * sizeof(T));
      for (int k = 0; k < theSize; k++)
        objects[k] = oldArray[k];

      theCapacity = newCapacity;

      free(oldArray);
    }

    T& operator[](int index)
    {
      return objects[index];
    }
    const T& operator[](int index) const
    {
      return objects[index];
    }
    bool empty() const
    {
      return size() == 0;
    }
    int size() const
    {
      return theSize;
    }
    int capacity() const
    {
      return theCapacity;
    }

    void push_back(T x)
    {
      if (theSize == theCapacity)
        reserve(2 * theCapacity + 1);
      objects[theSize++] = x;
    }

    const T& front() const
    {
      return objects[0];
    }
    T& front()
    {
      return objects[0];
    }

    void erase(int index)
    {
      for (int i = index; i < size() - 1; ++i)    // for each item that follows 'index'
        objects[i] = objects[i + 1];            // shift the item down one slot in memory
      --theSize;
    }

    void clear()
    {
      theSize = 0;
      theCapacity = 0;
      if (objects)
        free(objects);
      objects = 0;
    }

    iterator begin()
    {
      return objects;
    }
    iterator end()
    {
      return objects + theSize;
    }
    const_iterator begin() const
    {
      return objects;
    }
    const_iterator end() const
    {
      return objects + theSize;
    }

};

#endif /* VECTOR_H_ */
