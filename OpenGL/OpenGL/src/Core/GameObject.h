#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:
	std::string name;
	bool activeSelf = true; 

	Transform* transform = nullptr;

	GameObject(const std::string& name = "GameObject");

	template<typename T, typename... Args>
	T* AddComponent(Args&&... args);

	template<typename T>
	T* GetComponent();

	void Awake();
	void Start();
	void Update(float deltaTime);
	void Draw(Shader& shader);

private:
	std::vector<std::shared_ptr<Component>> components;
};

