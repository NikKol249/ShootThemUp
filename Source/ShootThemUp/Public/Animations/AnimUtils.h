#pragma once

class AnimUtils
{
public:
	template<typename T>
	static T* FindNotifyByClass(UAnimSequenceBase* Animatinon)
	{
		if(!Animatinon) return nullptr;
	
		const auto NotifyEvents = Animatinon->Notifies;
		for(auto NotifyEvent : NotifyEvents)
		{
			auto AnimNotify = Cast<T>(NotifyEvent.Notify);
			if(AnimNotify)
			{
				return AnimNotify;
			}
		}
		return nullptr;
	}
};