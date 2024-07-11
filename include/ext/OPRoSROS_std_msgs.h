#ifndef OPROS_ROS_STD_MSGS_H
#define OPROS_ROS_STD_MSGS_H
#include <ros/ros.h>
#include <std_msgs/String.h>

// forward declaration

namespace opros
{
namespace archive{
class oarchive;
class iarchive;
}
}

void save_this(opros::archive::oarchive &ar, std_msgs::String &t);
void load_this(opros::archive::iarchive &ar, std_msgs::String &t);
std::string typeName(std_msgs::String *t);


// inline function definition


#include <archive/opros_archive.h>
inline void save_this(opros::archive::oarchive &ar, std_msgs::String &t)
{
	ar << t.data;
}
inline void load_this(opros::archive::iarchive &ar, std_msgs::String &t)
{
	ar >> t.data;
}
inline std::string typeName(std_msgs::String *t)
{
	return "std_msgs::String";
}


#endif
