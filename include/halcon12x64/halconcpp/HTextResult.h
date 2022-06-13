/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 12.0
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HTEXTRESULT
#define HCPP_HTEXTRESULT

namespace HalconCpp
{

// Represents an instance of a text segmentations result.
class LIntExport HTextResult : public HToolBase
{

public:

  // Create an uninitialized instance
  HTextResult():HToolBase() {}

  // Copy constructor
  HTextResult(const HTextResult& source) : HToolBase(source) {}

  // Create HTextResult from handle, taking ownership
  explicit HTextResult(Hlong handle);

  // Set new handle, taking ownership
  void SetHandle(Hlong handle);



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // find_text: Find text in an image.
  explicit HTextResult(const HImage& Image, const HTextModel& TextModel);




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Clear all text results.
  static void ClearAllTextResults();

  // Query an iconic value of a text segmentation result.
  HObject GetTextObject(const HTuple& ResultName) const;

  // Query an iconic value of a text segmentation result.
  HObject GetTextObject(const HString& ResultName) const;

  // Query an iconic value of a text segmentation result.
  HObject GetTextObject(const char* ResultName) const;

  // Query a control value of a text segmentation result.
  HTuple GetTextResult(const HTuple& ResultName) const;

  // Query a control value of a text segmentation result.
  HTuple GetTextResult(const HString& ResultName) const;

  // Query a control value of a text segmentation result.
  HTuple GetTextResult(const char* ResultName) const;

  // Find text in an image.
  void FindText(const HImage& Image, const HTextModel& TextModel);

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HToolArrayRef;

typedef HToolArrayRef<HTextResult> HTextResultArrayRef;
typedef HSmartPtr< HTextResultArrayRef > HTextResultArrayPtr;


// Represents multiple tool instances
class LIntExport HTextResultArray : public HToolArray
{

public:

  // Create empty array
  HTextResultArray();

  // Create array from native array of tool instances
  HTextResultArray(HTextResult* classes, Hlong length);

  // Copy constructor
  HTextResultArray(const HTextResultArray &tool_array);

  // Destructor
  virtual ~HTextResultArray();

  // Assignment operator
  HTextResultArray &operator=(const HTextResultArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HTextResult* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HTextResultArrayPtr *mArrayPtr;
};

}

#endif
