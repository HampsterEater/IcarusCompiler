// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "Class.h"
#include <string>

#include "String.h"

//#define DEBUG_OUTPUT

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------
std::vector<ClassDefinition*> g_classDefinitions;
MetaDataDefinition* g_globalDefinition;

std::list<ClassObjectHeader*> g_gcObjects;
std::list<ClassObjectHeader*> g_gcZeroRefObjects;
unsigned long long g_gcTotalAllocatedMemory;
unsigned long long g_gcAllocatedMemory;
unsigned long long g_gcFreedMemory;
unsigned long long g_gcZeroRefMemory;
unsigned long long g_gcZeroRefObjectCount;
bool g_gcPaused;

// --------------------------------------------------------------------------------
//  Internal functions.
// --------------------------------------------------------------------------------
/*ClassDefinition* GetClassByName(std::string name)
{

}
ClassDefinition* GetFunctionByName(std::string name, ClassDefinition* parent)
{

}
ClassDefinition* GetVariableByName(std::string name, ClassDefinition* parent)
{
	VariableDefinition* varList = g_globalDefinition->variables;		
	if (parent != NULL)
		varList = parent->variables;

	whi
}*/

MetaDataDefinition* GetMetaDataByName(const char* name, MetaDataDefinition* parent, int symbolType)
{
	if (parent == NULL)
		parent = g_globalDefinition;

	MetaDataDefinition** children = parent->childList;
	while (true)
	{
		MetaDataDefinition* child = *(children++);
		if (child == NULL) break;

		if ((symbolType == META_TYPE_ANY || child->symbolType == symbolType) &&
			stricmp(child->ptrToName, name) == 0)
			return child;
	}
}

void SetVariableValueByte(VariableDefinition* variable, ICR_BYTE value, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: *((ICR_BYTE*)variable->variableAddress) = value; break;
	case VT_FIELD:  *((ICR_BYTE*)(thisPtr + variable->classOffset)) = value; break;
	}
}
void SetVariableValueShort(VariableDefinition* variable, ICR_SHORT value, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: *((ICR_SHORT*)variable->variableAddress) = value; break;
	case VT_FIELD:  *((ICR_SHORT*)(thisPtr + variable->classOffset)) = value; break;
	}
}
void SetVariableValueInt(VariableDefinition* variable, ICR_INT value, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: *((ICR_INT*)variable->variableAddress) = value; break;
	case VT_FIELD:  *((ICR_INT*)(thisPtr + variable->classOffset)) = value; break;
	}
}
void SetVariableValueLong(VariableDefinition* variable, ICR_LONG value, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: *((ICR_LONG*)variable->variableAddress) = value; break;
	case VT_FIELD:  *((ICR_LONG*)(thisPtr + variable->classOffset)) = value; break;
	}
}
void SetVariableValueFloat(VariableDefinition* variable, ICR_FLOAT value, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: *((ICR_FLOAT*)variable->variableAddress) = value; break;
	case VT_FIELD:  *((ICR_FLOAT*)(thisPtr + variable->classOffset)) = value; break;
	}
}
void SetVariableValueDouble(VariableDefinition* variable, ICR_DOUBLE value, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: *((ICR_DOUBLE*)variable->variableAddress) = value; break;
	case VT_FIELD:  *((ICR_DOUBLE*)(thisPtr + variable->classOffset)) = value; break;
	}
}



ICR_BYTE GetVariableValueByte(VariableDefinition* variable, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: return *((ICR_BYTE*)variable->variableAddress); break;
	case VT_FIELD:  return *((ICR_BYTE*)(thisPtr + variable->classOffset)); break;
	}
}
ICR_SHORT GetVariableValueShort(VariableDefinition* variable, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: return *((ICR_SHORT*)variable->variableAddress); break;
	case VT_FIELD:  return *((ICR_SHORT*)(thisPtr + variable->classOffset)); break;
	}
}
ICR_INT GetVariableValueInt(VariableDefinition* variable, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: return *((ICR_INT*)variable->variableAddress); break;
	case VT_FIELD:  return *((ICR_INT*)(thisPtr + variable->classOffset)); break;
	}
}
ICR_LONG GetVariableValueLong(VariableDefinition* variable, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: return *((ICR_LONG*)variable->variableAddress); break;
	case VT_FIELD:  return *((ICR_LONG*)(thisPtr + variable->classOffset)); break;
	}
}
ICR_FLOAT GetVariableValueFloat(VariableDefinition* variable, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: return *((ICR_FLOAT*)variable->variableAddress); break;
	case VT_FIELD:  return *((ICR_FLOAT*)(thisPtr + variable->classOffset)); break;
	}
}
ICR_DOUBLE GetVariableValueDouble(VariableDefinition* variable, ICR_INT thisPtr)
{
	switch (variable->variableType)
	{
	case VT_GLOBAL: return *((ICR_DOUBLE*)variable->variableAddress); break;
	case VT_FIELD:  return *((ICR_DOUBLE*)(thisPtr + variable->classOffset)); break;
	}
}

// --------------------------------------------------------------------------------
//  Exported functions.
// --------------------------------------------------------------------------------

ICR_INT icr__Class_ToString(ICR_INT ptrToObject)
{
	ClassObjectHeader* objPtr = (ClassObjectHeader*)(ptrToObject - ClassHeaderSize);
	int (*func)(int) = (int(*)(int))(objPtr->classDefinition->ptrToVFTable[VFTABLE_TOSTRING_ENTRY]);
	return func(ptrToObject);
}

void icr__RegisterClass(ClassDefinition* ptrToClassDefinition)
{
#ifdef DEBUG_OUTPUT
	printf("Register class type:\n");
	printf("  Declared at: %s(%i:%i) \n", ptrToClassDefinition->ptrToDeclaredFileName, ptrToClassDefinition->declaredFileLine, ptrToClassDefinition->declaredFileOffset);
	printf("  Class name: %s\n", ptrToClassDefinition->ptrToName);
	printf("  Memory data size: %i\n", ptrToClassDefinition->memoryDataSize);
#endif

	// Check for any linked in classes.
	// TODO: Case less compare.
	if (std::string(ptrToClassDefinition->ptrToName) == "string")
	{
		g_stringClassTableIndex = (int)g_classDefinitions.size();
#ifdef DEBUG_OUTPUT
	printf("Registered as built in type!\n");
#endif
	}

#ifdef DEBUG_OUTPUT
	printf("\n");
#endif

	g_classDefinitions.push_back(ptrToClassDefinition);
}

void icr__RegisterGlobal(MetaDataDefinition* ptrToGlobalDefinition)
{
	g_globalDefinition = ptrToGlobalDefinition;
	icr__RegisterMetaData(g_globalDefinition);
}

void icr__RegisterMetaData(MetaDataDefinition* metaData)
{
	// Register depending on type.
	switch (metaData->symbolType)
	{
		case META_TYPE_VARIABLE:
		{
			break;
		}
		case META_TYPE_FUNCTION:
		{
			break;
		}
		case META_TYPE_CLASS:
		{
			icr__RegisterClass((ClassDefinition*)metaData);
			break;
		}
	}

	// Register the children!
	MetaDataDefinition** children = metaData->childList;
	while (true)
	{
		MetaDataDefinition* child = *(children++);
		if (child == NULL) break;
		icr__RegisterMetaData(child);
	}
}

ICR_INT icr__AllocateObjectByClass(ClassDefinition* classType)
{

#ifdef DEBUG_OUTPUT
	printf("Initializing instance of %s\n", classType->ptrToName);
#endif

	// Allocate the memory for the class.
	void* classData = malloc(ClassHeaderSize + classType->memoryDataSize);
	memset(classData, 0, ClassHeaderSize + classType->memoryDataSize);

	// Track allocated data.
	g_gcTotalAllocatedMemory += ClassHeaderSize + classType->memoryDataSize;
	g_gcAllocatedMemory += ClassHeaderSize + classType->memoryDataSize;

	// Set the header values of the object.
	ClassObjectHeader* objPtr = (ClassObjectHeader*)classData;
	objPtr->allocatedMemory = ClassHeaderSize + classType->memoryDataSize;
	objPtr->classDefinition = classType;
	objPtr->classDefinitionIndex = classType->classIndex;
	objPtr->disposing = false;

	ICR_INT retVal = ((int)classData) + ClassHeaderSize;
	
	// Push the object allocated into the GC's list.
	g_gcObjects.push_back(objPtr);
	g_gcZeroRefObjects.push_back(objPtr);
	g_gcZeroRefMemory += objPtr->allocatedMemory;
	g_gcZeroRefObjectCount++;
	
	// Invoke the constructor.
	if (classType->internalConstructMethod != NULL)
	{
		g_gcPaused = true;
		
		// Call real-worker :3
		objPtr->classDefinition->internalConstructMethod(retVal);

		g_gcPaused = false;
	}

#ifdef DEBUG_OUTPUT
	printf("Address of allocated data %i\n", ((int)classData));
	printf("\n");
#endif
	
	return retVal; // Increase address by header size so we point directly at user data.
}

ICR_INT icr__AllocateObject(ICR_INT classTableIndex)
{
	ClassDefinition* classType = g_classDefinitions.at(classTableIndex);
	return icr__AllocateObjectByClass(classType);
}

void icr__DeallocateObject(ICR_INT ptrToObject)
{
	// Get the object header value.
	ClassObjectHeader* objPtr = (ClassObjectHeader*)(ptrToObject - ClassHeaderSize);
	objPtr->disposing = true;

	// Invoke the desstructor.
	if (objPtr->classDefinition->internalDisposeMethod != NULL)
	{
		g_gcPaused = true;

		// Call user-defined dispose method.
		void (*func)(int) = (void(*)(int))(objPtr->classDefinition->ptrToVFTable[VFTABLE_DELETE_ENTRY]);
		func(ptrToObject);

		// Call the underlying method that does the real horsework :3.
		objPtr->classDefinition->internalDisposeMethod(ptrToObject);

		g_gcPaused = false;
		
		// *sigh* Someones assigned more references.
		if (objPtr->referenceCount > 0)
		{
			objPtr->disposing = false;
			return;
		}
	}

	// Track dallocated data.
	g_gcAllocatedMemory -= objPtr->allocatedMemory;
	g_gcFreedMemory += objPtr->allocatedMemory;
	
	// If its a string, deallocate the string data too.
	if (objPtr->classDefinitionIndex == g_stringClassTableIndex)
	{
		StringObjectData* stringData = (StringObjectData*)((int*)ptrToObject);
		if (stringData->ptrToData != NULL)
		{
			delete[] stringData->ptrToData;
		}
		else
		{
#ifdef DEBUG_OUTPUT
			printf("Tired to free string data, but is equal to null.\n", ptrToObject);
#endif
		}
	}

	// Free the memory that makes the object up.
	free(objPtr);

	// Pop object off the gc object list.
	g_gcObjects.remove(objPtr);
	
#ifdef DEBUG_OUTPUT
	printf("Deallocated object instance %i\n", ptrToObject);
	printf("\n");
#endif
}

void icr__IncrementRefCount(ICR_INT ptrToObject)
{
	if (ptrToObject == 0)
		return;

	ClassObjectHeader* objPtr = (ClassObjectHeader*)(ptrToObject - ClassHeaderSize);
	if (objPtr->disposing == true)
		return;

	if (objPtr->referenceCount == 0)
	{
		g_gcZeroRefMemory -= objPtr->allocatedMemory;
		g_gcZeroRefObjectCount--;
		g_gcZeroRefObjects.remove(objPtr);
	}

	objPtr->referenceCount++;

#ifdef DEBUG_OUTPUT
	printf("Increment refs of %i, now %i\n", ((int)objPtr), objPtr->referenceCount);
#endif
}

void icr__DecrementRefCount(ICR_INT ptrToObject)
{
	if (ptrToObject == 0)
		return;

	ClassObjectHeader* objPtr = (ClassObjectHeader*)(ptrToObject - ClassHeaderSize);
	if (objPtr->disposing == true)
		return;
	
	objPtr->referenceCount--;
	
	if (objPtr->referenceCount <= 0)
	{
		g_gcZeroRefMemory += objPtr->allocatedMemory;
		g_gcZeroRefObjectCount++;
		g_gcZeroRefObjects.push_back(objPtr);
	}
	
#ifdef DEBUG_OUTPUT
	printf("Decrement refs of %i, now %i\n", ((int)objPtr), objPtr->referenceCount);
#endif
}

ICR_INT icr__ExplicitCast(ICR_INT destinationClassIndex, ICR_INT ptrToObject)
{
	if (ptrToObject == 0)
		return 0;

	ClassObjectHeader* objPtr = (ClassObjectHeader*)(ptrToObject - ClassHeaderSize);
	ClassDefinition* srcClassType = g_classDefinitions.at(objPtr->classDefinitionIndex); // TODO: Don't do .at, use array or something similar. Quicker.
	ClassDefinition* destClassType = g_classDefinitions.at(destinationClassIndex); // TODO: Don't do .at, use array or something similar. Quicker.

	// Check upwards in hierarchy.
	while (srcClassType != NULL)
	{
		if (srcClassType == destClassType)
			return ptrToObject;

		if (srcClassType->superClassIndex == -1)
			break;

		srcClassType = g_classDefinitions.at(srcClassType->superClassIndex);
	}

	// Can't convert :(.
	return 0;
}

void icr__BeginGC()
{
#ifdef DEBUG_OUTPUT
	printf("Garbage collector starting up.\n");
#endif
}

void icr__FinishGC()
{
#ifdef DEBUG_OUTPUT
	printf("Garbage Collector Shutting Down, Deallocating Objects ..\n");
#endif

	std::list<ClassObjectHeader*> deadObjectList;

	// Go through each object and find any dead objects.
	std::list<ClassObjectHeader*>::iterator iterator;
	for (iterator = g_gcObjects.begin(); iterator != g_gcObjects.end(); iterator++)
	{
		ClassObjectHeader* objPtr = *iterator;
#ifdef DEBUG_OUTPUT
		printf("- Marked object for deletion: %i (refs:%i)\n", (int)((int*)objPtr), objPtr->referenceCount);
#endif
		deadObjectList.push_back(objPtr);
	}

	// Deallocate dead objects.
	while (deadObjectList.size() > 0)
	{	
		ClassObjectHeader* objPtr = deadObjectList.back();
		deadObjectList.pop_back();

		icr__DeallocateObject( ((int)((int*)objPtr)) + ClassHeaderSize );
	}
	
#ifdef DEBUG_OUTPUT
	printf("Finished Deallocating Objects\n");
#endif
}

void icr__ExecuteGC()
{
	if ((g_gcZeroRefMemory < GC_DEALLOCATE_DEAD_MEMORY_THRESHOLD &&
		g_gcZeroRefObjectCount < GC_DEALLOCATE_DEAD_OBJECT_THRESHOLD) || g_gcPaused == true)
		return;

	while (g_gcZeroRefObjects.size() > 0)
	{	
		ClassObjectHeader* objPtr = g_gcZeroRefObjects.back();
		g_gcZeroRefObjects.pop_back();
		
		icr__DeallocateObject( ((int)((int*)objPtr)) + ClassHeaderSize );
	}

	g_gcZeroRefMemory = 0;
	g_gcZeroRefObjectCount = 0;
	g_gcZeroRefObjects.clear();
}