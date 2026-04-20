#include "GameObject.h"

GameObject::GameObject(const std::string& name)
{
	auto t = std::make_shared<Transform>();
	t->gameObject = this;
	t->transform = t.get(); 
	components.push_back(t);
	transform = t.get();
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

