#pragma once
class GameObject;
class Transform;
class Shader;

class Component
{
public:
	GameObject* gameObject = nullptr;
	Transform* transform = nullptr;

	bool enabled = true;

	Component();
	~Component();

	virtual void Awake();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void Draw(Shader& shader) {}
};
