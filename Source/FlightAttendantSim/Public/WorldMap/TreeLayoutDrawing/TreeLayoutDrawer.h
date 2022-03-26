// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UMapNode;

class ITreeLayoutDrawer
{
public:
	virtual void MakeGridLayout(UMapNode* TreeRoot) = 0;

	virtual ~ITreeLayoutDrawer() = default;
};
