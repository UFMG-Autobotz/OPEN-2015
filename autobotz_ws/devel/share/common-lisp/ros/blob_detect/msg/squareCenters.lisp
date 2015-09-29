; Auto-generated. Do not edit!


(cl:in-package blob_detect-msg)


;//! \htmlinclude squareCenters.msg.html

(cl:defclass <squareCenters> (roslisp-msg-protocol:ros-message)
  ((centers
    :reader centers
    :initarg :centers
    :type (cl:vector geometry_msgs-msg:Point)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point :initial-element (cl:make-instance 'geometry_msgs-msg:Point))))
)

(cl:defclass squareCenters (<squareCenters>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <squareCenters>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'squareCenters)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name blob_detect-msg:<squareCenters> is deprecated: use blob_detect-msg:squareCenters instead.")))

(cl:ensure-generic-function 'centers-val :lambda-list '(m))
(cl:defmethod centers-val ((m <squareCenters>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader blob_detect-msg:centers-val is deprecated.  Use blob_detect-msg:centers instead.")
  (centers m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <squareCenters>) ostream)
  "Serializes a message object of type '<squareCenters>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'centers))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'centers))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <squareCenters>) istream)
  "Deserializes a message object of type '<squareCenters>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<squareCenters>)))
  "Returns string type for a message object of type '<squareCenters>"
  "blob_detect/squareCenters")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'squareCenters)))
  "Returns string type for a message object of type 'squareCenters"
  "blob_detect/squareCenters")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<squareCenters>)))
  "Returns md5sum for a message object of type '<squareCenters>"
  "187ed2919801ff6e269ae8a0ca378673")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'squareCenters)))
  "Returns md5sum for a message object of type 'squareCenters"
  "187ed2919801ff6e269ae8a0ca378673")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<squareCenters>)))
  "Returns full string definition for message of type '<squareCenters>"
  (cl:format cl:nil "geometry_msgs/Point[] centers~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'squareCenters)))
  "Returns full string definition for message of type 'squareCenters"
  (cl:format cl:nil "geometry_msgs/Point[] centers~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <squareCenters>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'centers) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <squareCenters>))
  "Converts a ROS message object to a list"
  (cl:list 'squareCenters
    (cl:cons ':centers (centers msg))
))
