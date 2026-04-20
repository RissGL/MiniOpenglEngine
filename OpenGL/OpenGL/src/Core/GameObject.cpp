#include "GameObject.h"

GameObject::GameObject(const std::string& name)
{
}
void GameObject::Awake()
{
	for (auto& comp : components)
	{
		if (comp->enabled)
		{
			comp->Awake();
		}
	}
}
void GameObject::Start()
{
	for (auto& comp : components)
	{
		if (comp->enabled)
		{
			comp->Start();
		}
	}
}
void GameObject::Update(float deltaTime)
{
	if (!activeSelf) return;
	for (auto& comp : components)
	{
		if (comp->enabled)
		{
			comp->Update(deltaTime);
		}
	}
}
void GameObject::Draw(Shader& shader)
{
	if (!activeSelf) return;
	for (auto& comp : components) if (comp->enabled) comp->Draw(shader);
}
template<typename T, typename ...Args>
T* GameObject::AddComponent(Args && ...args)
{
	static_assert(std::is_base_of<Component, T>::value, "´íÎó: ½Å±¾±ØĞë¼Ì³Ğcommpent");
	std::shared_ptr <T> newComp = std::make_shared<T>(std::forward<Args>(args)...);
	newComp->gameObject = this;
	newComp->transform = this;

	components.push_back(newComp);
	newComp->Awake();
	return newComp.get();
}

template<typename T>
T* GameObject::GetComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "´íÎó: ½Å±¾±ØĞë¼Ì³Ğcommpent");

	for (auto& comp : components) {
		T* target = dynamic_cast<T*>(comp.get());
		if (target) return target;
	}
	return nullptr;
}
