; Auto-generated. Do not edit!


(cl:in-package blob_detect-msg)


;//! \htmlinclude feature.msg.html

(cl:defclass <feature> (roslisp-msg-protocol:ros-message)
  ((centers
    :reader centers
    :initarg :centers
    :type (cl:vector geometry_msgs-msg:Point)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point :initial-element (cl:make-instance 'geometry_msgs-msg:Point))))
)

(cl:defclass feature (<feature>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <feature>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'feature)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name blob_detect-msg:<feature> is deprecated: use blob_detect-msg:feature instead.")))

(cl:ensure-generic-function 'centers-val :lambda-list '(m))
(cl:defmethod centers-val ((m <feature>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader blob_detect-msg:centers-val is deprecated.  Use blob_detect-msg:centers instead.")
  (centers m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <feature>) ostream)
  "Serializes a message object of type '<feature>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'centers))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'centers))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <feature>) istream)
  "Deserializes a message object of type '<feature>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'centers) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'centers)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<feature>)))
  "Returns string type for a message object of type '<feature>"
  "blob_detect/feature")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'feature)))
  "Returns string type for a message object of type 'feature"
  "blob_detect/feature")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<feature>)))
  "Returns md5sum for a message object of type '<feature>"
  "187ed2919801ff6e269ae8a0ca378673")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'feature)))
  "Returns md5sum for a message object of type 'feature"
  "187ed2919801ff6e269ae8a0ca378673")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<feature>)))
  "Returns full string definition for message of type '<feature>"
  (cl:format cl:nil "geometry_msgs/Point[] centers~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'feature)))
  "Returns full string definition for message of type 'feature"
  (cl:format cl:nil "geometry_msgs/Point[] centers~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <feature>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'centers) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <feature>))
  "Converts a ROS message object to a list"
  (cl:list 'feature
    (cl:cons ':centers (centers msg))
))
