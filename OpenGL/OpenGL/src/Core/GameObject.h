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
	const std::vector<std::shared_ptr<Component>>& GetComponents() const { return components; }


	GameObject(const std::string& name = "GameObject");

	template<typename T, typename ...Args>
	T* AddComponent(Args && ...args)
	{
		static_assert(std::is_base_of<Component, T>::value, "´íÎó: ½Å±¾±ØÐë¼Ì³Ðcommpent");
		std::shared_ptr <T> newComp = std::make_shared<T>(std::forward<Args>(args)...);
		newComp->gameObject = this;
		newComp->transform = this->transform;

		components.push_back(newComp);
		newComp->Awake();
		return newComp.get();
	}

	template<typename T>
	T* GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "´íÎó: ½Å±¾±ØÐë¼Ì³Ðcommpent");

		for (auto& comp : components) {
			T* target = dynamic_cast<T*>(comp.get());
			if (target) return target;
		}
		return nullptr;
	}

	void Awake();
	void Start();
	void Update(float deltaTime);
	void Draw(Shader& shader);

	std::string GetName() const;

private:
	std::vector<std::shared_ptr<Component>> components;
};

