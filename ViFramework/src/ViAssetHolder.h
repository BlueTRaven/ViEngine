#pragma once

#include <string>
#include <map>
#include <vector>

#include "ViVifParser.h"
#include "ViVifLine.h"
#include "ViAssetDefinition.h"

template<typename T>
class ViAssetHolder
{
public:
	ViAssetHolder();
	ViAssetHolder(std::string vifpath);

	void Find(std::string vifpath);

	virtual T GetAsset(std::string name);

	virtual void UnloadAsset(std::string aName);

	virtual void UnloadAllAssets();

protected:
	virtual void SetName(ViAssetDefinition& definition);

	virtual T LoadAsset(ViAssetDefinition aDefinition) = 0;

	ViVifLine FindLine(ViAssetDefinition aDefinition, std::string aFirstName);

	vi_property_get_named(std::string, mBasePath, BasePath);

private:
	std::map<std::string, T> mAssets;
	std::map<std::string, ViAssetDefinition> mAssetsDefinitions;

	void SetBasePath(std::string aBasePath);

	std::string mBasePath;

	bool mFound;

	bool mDirty;	//can be used to reload all assets if already loaded

	bool mCached = true;	//is the asset cached? Some assets we want to reload and generate a new version of every time (shaders)
};

template<typename T>
ViAssetHolder<T>::ViAssetHolder() : 
	mFound(false), 
	mDirty(false), 
	mCached(true),
	mAssets({}),
	mAssetsDefinitions({}),
	mBasePath("")
{ };

template<typename T>
ViAssetHolder<T>::ViAssetHolder(std::string aPath) 
{
	Find(aPath);
};

template<typename T>
void ViAssetHolder<T>::Find(std::string aPath) 
{
	ViVifParser parser(aPath);

	vi_assert(parser.GetValid(), "Error: could not find asset holder vif at " + aPath + ".");

	SetBasePath(aPath);

	std::vector<ViVifLine> assetLine;
	int index = 0;
	for (ViVifLine line : parser.GetLines())
	{
		if (line.mIsEmpty)
		{
			ViAssetDefinition definition;
			definition.definition = assetLine;
			definition.hasFound = false;

			SetName(definition);

			mAssetsDefinitions.emplace(definition.name, definition);

			assetLine.clear();
			continue;
		}

		assetLine.push_back(line);
		index++;
	}

	if (assetLine.size() > 0)
	{
		ViAssetDefinition definition;
		definition.definition = assetLine;
		definition.hasFound = false;

		SetName(definition);

		mAssetsDefinitions.emplace(definition.name, definition);
	}

#if _DEBUG
	printf("Debug: Loaded %i asset definitions from vif %s.\n", (int)mAssetsDefinitions.size(), aPath.c_str());
#endif
};

template<typename T>
T ViAssetHolder<T>::GetAsset(std::string aName)
{
	if (mCached && mAssets.find(aName) != mAssets.end())
	{
		//if we're dirty, deleted the currently loaded asset; we'll then move on to reloading it below
		if (mDirty)
			delete mAssets[aName];
		else return mAssets[aName];
	}

	if (mAssetsDefinitions.find(aName) != mAssetsDefinitions.end())
	{
		T asset = LoadAsset(mAssetsDefinitions[aName]);

		if (mCached)
		{
			mAssetsDefinitions[aName].hasFound = true;
			mAssets.emplace(aName, asset);
		}

		return asset;
	}
	else
	{
		printf("Error: Could not get asset with name %s - no definition for that asset exists under that name.", aName.c_str());
		return nullptr;
	}
}

template<typename T>
void ViAssetHolder<T>::UnloadAsset(std::string aName)
{
	if (mCached && mAssets.find(aName) != mAssets.end())
	{
		//If the name was found, delete it.
		delete mAssets[aName];
		return;
	}
	//otherwise, we're attempting to unload something that was either never initialized or has already been unloaded.
	printf("Warning: Attempted to unload asset with name '%s' that has not been loaded.", aName.c_str());
}

template<typename T>
void ViAssetHolder<T>::UnloadAllAssets()
{
	for (auto asset : mAssets)
	{
		delete mAssets[asset.first];
	}
}

template<typename T>
void ViAssetHolder<T>::SetName(ViAssetDefinition& aDefinition)
{
	ViVifLine line = aDefinition.definition[0];
	
	vi_assert((line.mWords.size() > 0), "Error: first line of asset definition cannot be empty.");

	std::string type = line.mWords[0];
	std::string name = line.mWords[1];

	aDefinition.name = name;
	aDefinition.type = type;
}

template<typename T>
void ViAssetHolder<T>::SetBasePath(std::string aBasePath)
{
	size_t size = aBasePath.size();
	for (size_t i = size; i > 0; i--)
	{
		char c = aBasePath[i];
		if (c == '/' || c == '\\')
		{
			mBasePath = aBasePath.substr(0, i);
			break;
		}
	}
}

template<typename T>
ViVifLine ViAssetHolder<T>::FindLine(ViAssetDefinition aDefinition, std::string aFirstName)
{
	for (ViVifLine line : aDefinition.definition)
	{
		if (line.mWords[0] == aFirstName)
			return line;
	}

	return ViVifLine();
}