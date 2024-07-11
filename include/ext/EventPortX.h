/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 23
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : EventPort.h
 *
 *
 */

#ifndef EVENTPORTX_H_
#define EVENTPORTX_H_

#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <EventData.h>
#include <Component.h>


class EventCallBack;


#ifdef SUPPORT_ROS
#ifndef EVENT_ROS
#define EVENT_ROS 1
#endif

#include <ros/ros.h>

template <typename dataType>
class InputEventPort<dataType, EVENT_ROS> : public Port
{
	std::string m_topic;
	EventCallBack *m_owner;
	Component *m_component;

	ros::Subscriber m_sub;

public:
	InputEventPort()
	{
		m_owner = NULL;
		m_component = NULL;
		m_type = OPROS_EVENT;
	}

	InputEventPort(const std::string &nname)
	{
		m_type = OPROS_EVENT;
	}

	virtual ~InputEventPort()
	{
	}

	virtual void reset()
	{
	}

	virtual void setTopic(const std::string &name)
	{
		m_topic = name;
	}

	virtual std::string getTopic()
	{
		return m_topic;
	}

	void ros_callback(const dataType &pdd)
	{
		EventData<dataType> td;

		td.setContentData(pdd);

		if (m_owner!= NULL)
		{
			m_owner->onEvent(&td);
		}
		else if (m_component!= NULL)
		{
			m_owner->onEvent(&td);
		}

	}

	virtual void setOwner(Component *owner)
	{
		m_component = owner;
	}

	virtual void setOwner(EventCallBack *owner)
	{
		m_owner = owner;

		ros::NodeHandle nh_;

		m_sub = nh_.subscribe(m_topic, 1,&InputEventPort<ROS, dataType>::ros_callback, this);
	}
};
inline void eventFetch()
{
	ros::spinOnce();
}
#else 
inline void eventFetch()
{
}
#endif


#ifdef SUPPORT_ROS
#ifndef EVENT_ROS
#define EVENT_ROS 1
#endif
#include <ros/ros.h>
template <typename dataType>
class OutputEventPort<dataType, EVENT_ROS> : public Port
{
	std::string m_topic;
	EventCallBack *m_owner;

	ros::Publisher m_pub;

	bool m_init;

	void setup()
	{
		m_init=true;

		ros::NodeHandle nh_;

		m_pub = nh_.advertise<dataType>(m_topic, 1);
	}

public:
	OutputEventPort()
	{
		m_init = false;
		m_type = OPROS_EVENT;
	}
	
	OutputEventPort(const std::string &topicName) : m_topic(topicName)
	{
		m_type = OPROS_EVENT;
		setup();
	}

	OutputEventPort(const std::string &nname,const std::string &topicName) : m_topic(topicName)
	{
		m_type = OPROS_EVENT;
		setup();
	}

	virtual ~OutputEventPort()
	{
	}

	virtual void reset()
	{
	}


	virtual ReturnType push(const dataType &p)
	{
		if (m_init)
		{
			m_pub.publish(p);

			return OPROS_SUCCESS;
		}

		return OPROS_PRECONDITION_NOT_MET;
	}

	virtual void setTopic(const std::string &name)
	{
		m_topic = name;
		setup();
	}

	virtual std::string getTopic()
	{
		return m_topic;
	}
};

#endif

template <typename T,typename C, int n>
void setTopicCallBack(InputEventPort<T,n> &p, void (C::*func)(Event *p), C *classPtr, const std::string &topic)
{
        EventCallBack *t= make_EventCallBack<T,C>(func, classPtr);

		p.setTopic(topic);
        p.setOwner(t);
}

template <typename T,typename C, int n>
void setTopicCallBack(InputEventPort<T,n> &p, void (C::*func)(EventData<T> *p), C *classPtr, const std::string &topic)
{
        EventCallBack *t= make_EventCallBack<T,C>(func, classPtr);

		p.setTopic(topic);
        p.setOwner(t);
}

template <typename T,typename C, int n>
void setTopicCallBack(InputEventPort<T,n> &p, void (C::*func)(const std::string &mid, const T &indata), C *classPtr, const std::string &topic)
{
        EventCallBack *t= make_EventCallBack<T,C>(func, classPtr);

		p.setTopic(topic);
        p.setOwner(t);
}

template <typename T,typename C, int n>
void setTopicCallBack(InputEventPort<T,n> &p, void (C::*func)(const T &indata), C *classPtr, const std::string &topic)
{
        EventCallBack *t= make_EventCallBack<T,C>(func, classPtr);

		p.setTopic(topic);
        p.setOwner(t);
}

template <typename T,int n>
void setTopicCallBack(InputEventPort<T,n> &p, void (*func)(Event *p), const std::string &topic)
{
        EventCallBack *t= make_EventCallBack<T>(func);

		p.setTopic(topic);
        p.setOwner(t);
}

template <typename T,int n>
void setTopicCallBack(InputEventPort<T,n> &p, void (*func)(EventData<T> *p), const std::string &topic)
{
        EventCallBack *t= make_EventCallBack<T>(func);

		p.setTopic(topic);
        p.setOwner(t);
}

template <typename T,int n>
void setTopicCallBack(InputEventPort<T,n> &p, void (*func)(const std::string &mid, const T &indata), const std::string &topic)
{
        EventCallBack *t= make_EventCallBack<T>(func);

		p.setTopic(topic);
        p.setOwner(t);
}

template <typename T,int n>
void setTopicCallBack(InputEventPort<T,n> &p, void (*func)(const T &indata), const std::string &topic)
{
        EventCallBack *t= make_EventCallBack<T>(func);

		p.setTopic(topic);
        p.setOwner(t);
}

#define SET_TOPIC_CALLBACK(port, func, classPtr, topic) setTopicCallBack(port, &func, classPtr, topic)
#define SET_TOPIC_CALLBACK_EX(port, func, topic) setTopicCallBack(port, &func, topic)



#endif /* EVENTPORTX_H_ */
