#include <string>
#include <iostream>
#include "CDCollection.h"
using namespace std;

// Default constructor
// Creates an empty CDCollection with default array size (4)
CDCollection::CDCollection()
{
    const int MAX_SIZE = 4;
    numcds = 0;
    maxsize = MAX_SIZE;
    cds = new CD[maxsize];
}

// Helper method for copy constructor
// Performs deep copy of dynamic array
void CDCollection::CopyArray(const CDCollection& cat)
{
    numcds = cat.numcds;
    maxsize = cat.maxsize;
    cds = new CD[maxsize];
    //CD* newArray = cat.cds;
    for(int i = 0; i < cat.numcds; i++)
    {
        cds[i] = cat.cds[i];
    }
}

// Copy constructor
// Creates a new CDCollection object,
// performs a deep copy of the cat parameter's fields
CDCollection::CDCollection(const CDCollection& cat)
{
    CopyArray(cat);
}

// Destructor
// Releases all memory allocated to private pointer members
CDCollection::~CDCollection()
{
    delete[] cds;
}

// Insert - performs a set insertion with the CD catalogue
// Inserts CD and returns true if CD is not already in the collection
// Does not insert and returns false if a CD with a matching artist and album name
// already exists in the registry
// If the array is already filled before the insertion occurs, then
// first create a new dynamic array double the size of the old array,
// copy all CD objects from old array to new array,
// deallocate memory associated with the old array,
// and insert the new CD and return true.
// POST: collection contains CD
// PARAM: disc = item to be inserted, its fields should not be empty string
// NOTE: since overloaded assignment operator has not been defined,
// "insertion" will mean updating all fields of the CD at the appropriate index
bool CDCollection::Insert(CD disc)
{
	//Boolean variable used to identify whether CD is inserted into collection
	//This will be the return value
    bool insert = true;
	//Loop to search through collection for specified artist and album strings
    for(int i = 0; i < numcds; i++)
    {
		//If both artis and album strings are already present, together, in collection
        if(cds[i].GetArtist() == disc.GetArtist() && cds[i].GetAlbum() == disc.GetAlbum())
        {
			//CD will not be inserted if a copy is already present
            insert = false;
        }
    }
	//If no CD was found with matching artist and album string
    if(insert == true)
    {
		//Test if array is already full. If it is: double array size
        if(numcds == maxsize)
        {
            CD* oldcollection = cds;
            maxsize = 2*maxsize;
            cds = new CD[maxsize];
            for(int i = 0; i < numcds; i++)
            {
                cds[i] = oldcollection[i];
            }
            delete[] oldcollection;
        }
		//Insert CD at the end of the array
        cds[numcds] = disc;
        numcds++;
    }
    return insert;
}

// Remove - performs a set removal with the CD collection
// Removes a CD with all matching parameters if one exists in the collection
// Returns false if a CD with the same parameters does not exist in the collection
// POST: collection does not contain CD
// PARAM: disc = item to be removed, its fields should not be empty string
// NOTE: "removal" if successful will be done by updating the parameters of the
//CD at the appropriate index using the parameters of the CD in
//the last valid index of the collection; then decrements count and returns true;
bool CDCollection::Remove(CD disc)
{
	//Boolean variable used to identify whether CD is removed from collection
	//This will be the return value
    bool remove = false;
	//Integer to store location of CD, if present
    int CDLocation = 0;
	//Loop through current array, testing CD with matching artist and album strings
    for(int i = 0; i < numcds; i++)
    {
        if(cds[i].GetArtist() == disc.GetArtist() && cds[i].GetAlbum() == disc.GetAlbum())
        {
			//If CD found with both artist and album strings matching those specified
			//Set CD location to current index of loop
            CDLocation = i;
			//Specify that removal will take place
            remove = true;
			//Break from loop, no further search is needed as duplicates are not allowed
            break;
        }
    }
	//If removal is flagged
    if(remove == true)
    {
		//Update specified index with artist and album string from last indexed CD
		//Then decrement numcds, effectively removing the previous final index from collection
        cds[CDLocation].UpdateArtist(cds[numcds-1].GetArtist());
        cds[CDLocation].UpdateAlbum(cds[numcds-1].GetAlbum());
        numcds--;
    }
    return remove;
}

// Locates the array index of a CD with matching parameters
// Returns -1 if no CD with matching parameters exists in the collection
// PARAM: disc = item to be located, its fields should not be empty string
int CDCollection::Find(CD disc) const
{
	//Initialize CDLocation variable to 'no match found' value
    int CDLocation=-1;
	//Loop through collection array for CD with matching artist and album strings
    for(int i = 0; i < numcds; i++)
    {
        if(cds[i].GetArtist() == disc.GetArtist() && cds[i].GetAlbum() == disc.GetAlbum())
        {
			//If both strings matched, place index value in CDLocation variable to be returned
            CDLocation = i;
            break;
        }
    }
        return CDLocation;
}

// Removes all CDs from the collection with artist matching the provided argument
// Returns false if there are not matching CDs or input is empty string,
// otherwise returns true if at least one CD is removed
// Remaining items may appear in the array in any order, with the requirement that
// there are no "empty" spaces in the array (i.e. an array with 5 items must occupy indices 0-4)
// POST: collection contains no instances of CDs with the supplied artist name
// PARAM: dontlikeanymore = name of artist whose CDs should be removed from the collection
bool CDCollection::Boycott(string dontlikeanymore)
{
	//Boolean variable to track whether any CDs found by specified artist string
	//This will be the return value
    bool removed = false;
	//Test for empty string
    if(dontlikeanymore.empty())
    {
		//If empty, return removed variable with value 'false'
        return removed;
    }
    else
    {
		//Loop through collection array for matching artist string
		//Reverse iteration due to method of removal - removal is achieved by replacing
		//CD memeber variables at specified index with those of the CD located at the final index.
		//Reversing through collection array ensures that the final indexed CD is checked first to
		//ensure it does not contain the boycotted artist string
        for(int i = numcds-1; i >=0; i--)
        {
            if(cds[i].GetArtist() == dontlikeanymore)
            {
				//If artist string matches, the artist and album strings of the indexed CD are
				//replaced by those of the final indexed CD
                cds[i].UpdateArtist(cds[numcds-1].GetArtist());
                cds[i].UpdateAlbum(cds[numcds-1].GetAlbum());
                numcds--;
                removed = true;
            }
        }
        return removed;
    }
}

// Returns the number of CDs in the catalogue
int CDCollection::Count() const
{
    return numcds;
}

// Returns the set union of this and cat
// POST: union contains CD of this and cat, with no duplicate CDs (both parameters matching).
CDCollection CDCollection::Join(const CDCollection& cat) const
{
	//All CDs from catalogue provided as argument are placed in new catalogue
    CDCollection catJoin(cat);
	//Loop through catalogue which called Join function
    for(int i = 0; i < numcds; i++)
    {
		//Using Insert function will ensure duplicate CDs are not added to new catalogue
		//The method is provided within the CDCollection::Insert function decleration
        catJoin.Insert(CD(cds[i].GetArtist(), cds[i].GetAlbum()));
    }
    return catJoin;
}

// Returns the set intersection of this and cat
// POST: intersection contains CDs in both this and cat (both parameters matching).
CDCollection CDCollection::Common(const CDCollection& cat) const
{
	//Initialize string variables to hold artist and album strings to be compared
    string artist;
    string album;
    CDCollection catCommon;
	//Loop through catalogue array which called function
    for(int i = 0; i < numcds; i++)
    {
		//For each index, place artist and album strings in corresponding variables
        artist = cds[i].GetArtist();
        album = cds[i].GetAlbum();
		//Loop through catalogue array provided as argument, checking for CDs with 
		//artist and album string variables which match those specified in outter loop
        for(int j = 0; j < cat.numcds; j++)
        {
            if(cat.cds[j].GetArtist() == artist && cat.cds[j].GetAlbum() == album)
            {
				//If match found, place in the new catalogue array
                catCommon.Insert(CD(artist, album));
                break;
            }
        }
    }
    return catCommon;
}

// Returns the set difference of this and cat
// CDs in both catalogues must have a full set of matching parameters
// for a CD to be removed in the split.
// POST: difference contains CDs in this but not also in cat
CDCollection CDCollection::Split(const CDCollection& cat) const
{
	//Initialize string variables to hold artist and album strings to be compared
    string artist;
    string album;
	//Boolean variable to signal whether current CD is to be placed in catSplit 
    bool add = true;
    CDCollection catSplit;
	//Loop to test each CD in catalogue array which called function to each CD in 
	//catalogue array provided as argument
    for(int i = 0; i < numcds; i++)
    {
		//Set add to true as default, this will only be made false if a match is found
        add = true;
		//Place artist and album string fields from currently indexed CD into test variables
        artist = cds[i].GetArtist();
        album = cds[i].GetAlbum();
		//Loop through argument catalogue array for CDs with artist and album variables which both 
		//match test variables
        for(int j = 0; j < cat.numcds; j++)
        {
            if(cat.cds[j].GetArtist() == artist && cat.cds[j].GetAlbum() == album)
            {
				//If match found, do not add to catSplit array
                add = false;
                break;
            }
        }
        if(add == true)
        {
			//If no match is found, CD is exclusive to calling array, add to catSplit
            catSplit.Insert(CD(artist, album));
        }
    }
	//Loop to test each CD in argument catalogue array to each CD in 
	//calling catalogue array 
    for(int i = 0; i < cat.numcds; i++)
    {
		//Set add to true as default, this will only be made false if a match is found
        add = true;
		//Place artist and album string fields from currently indexed CD into test variables
        artist = cat.cds[i].GetArtist();
        album = cat.cds[i].GetAlbum();
		//Loop through calling catalogue array for CDs with artist and album variables which both 
		//match test variables
        for(int j = 0; j < numcds; j++)
        {
            if(cds[j].GetArtist() == artist && cds[j].GetAlbum() == album)
            {
				//If match found, do not add to catSplit array
                add = false;
                break;
            }
        }
        if(add == true)
        {
			//If no match is found, CD is exclusive to argument array, add to catSplit
            catSplit.Insert(CD(artist, album));
        }
    }
    return catSplit;
}
