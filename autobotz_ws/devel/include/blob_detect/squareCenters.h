// Generated by gencpp from file blob_detect/squareCenters.msg
// DO NOT EDIT!


#ifndef BLOB_DETECT_MESSAGE_SQUARECENTERS_H
#define BLOB_DETECT_MESSAGE_SQUARECENTERS_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <geometry_msgs/Point.h>

namespace blob_detect
{
template <class ContainerAllocator>
struct squareCenters_
{
  typedef squareCenters_<ContainerAllocator> Type;

  squareCenters_()
    : centers()  {
    }
  squareCenters_(const ContainerAllocator& _alloc)
    : centers(_alloc)  {
    }



   typedef std::vector< ::geometry_msgs::Point_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::geometry_msgs::Point_<ContainerAllocator> >::other >  _centers_type;
  _centers_type centers;




  typedef boost::shared_ptr< ::blob_detect::squareCenters_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::blob_detect::squareCenters_<ContainerAllocator> const> ConstPtr;

}; // struct squareCenters_

typedef ::blob_detect::squareCenters_<std::allocator<void> > squareCenters;

typedef boost::shared_ptr< ::blob_detect::squareCenters > squareCentersPtr;
typedef boost::shared_ptr< ::blob_detect::squareCenters const> squareCentersConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::blob_detect::squareCenters_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::blob_detect::squareCenters_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace blob_detect

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'geometry_msgs': ['/opt/ros/indigo/share/geometry_msgs/cmake/../msg'], 'blob_detect': ['/home/husky/open-2015/autobotz_ws/src/blob_detect/msg'], 'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::blob_detect::squareCenters_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::blob_detect::squareCenters_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::blob_detect::squareCenters_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::blob_detect::squareCenters_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::blob_detect::squareCenters_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::blob_detect::squareCenters_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::blob_detect::squareCenters_<ContainerAllocator> >
{
  static const char* value()
  {
    return "187ed2919801ff6e269ae8a0ca378673";
  }

  static const char* value(const ::blob_detect::squareCenters_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x187ed2919801ff6eULL;
  static const uint64_t static_value2 = 0x269ae8a0ca378673ULL;
};

template<class ContainerAllocator>
struct DataType< ::blob_detect::squareCenters_<ContainerAllocator> >
{
  static const char* value()
  {
    return "blob_detect/squareCenters";
  }

  static const char* value(const ::blob_detect::squareCenters_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::blob_detect::squareCenters_<ContainerAllocator> >
{
  static const char* value()
  {
    return "geometry_msgs/Point[] centers\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
";
  }

  static const char* value(const ::blob_detect::squareCenters_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::blob_detect::squareCenters_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.centers);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct squareCenters_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::blob_detect::squareCenters_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::blob_detect::squareCenters_<ContainerAllocator>& v)
  {
    s << indent << "centers[]" << std::endl;
    for (size_t i = 0; i < v.centers.size(); ++i)
    {
      s << indent << "  centers[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::Point_<ContainerAllocator> >::stream(s, indent + "    ", v.centers[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // BLOB_DETECT_MESSAGE_SQUARECENTERS_H
