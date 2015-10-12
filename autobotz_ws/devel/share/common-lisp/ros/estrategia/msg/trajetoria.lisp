; Auto-generated. Do not edit!


(cl:in-package estrategia-msg)


;//! \htmlinclude trajetoria.msg.html

(cl:defclass <trajetoria> (roslisp-msg-protocol:ros-message)
  ((pontos
    :reader pontos
    :initarg :pontos
    :type (cl:vector geometry_msgs-msg:Pose2D)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Pose2D :initial-element (cl:make-instance 'geometry_msgs-msg:Pose2D))))
)

(cl:defclass trajetoria (<trajetoria>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <trajetoria>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'trajetoria)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name estrategia-msg:<trajetoria> is deprecated: use estrategia-msg:trajetoria instead.")))

(cl:ensure-generic-function 'pontos-val :lambda-list '(m))
(cl:defmethod pontos-val ((m <trajetoria>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader estrategia-msg:pontos-val is deprecated.  Use estrategia-msg:pontos instead.")
  (pontos m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <trajetoria>) ostream)
  "Serializes a message object of type '<trajetoria>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'pontos))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'pontos))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <trajetoria>) istream)
  "Deserializes a message object of type '<trajetoria>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'pontos) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'pontos)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Pose2D))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<trajetoria>)))
  "Returns string type for a message object of type '<trajetoria>"
  "estrategia/trajetoria")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'trajetoria)))
  "Returns string type for a message object of type 'trajetoria"
  "estrategia/trajetoria")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<trajetoria>)))
  "Returns md5sum for a message object of type '<trajetoria>"
  "2cb8d6f7fd8051a8b0b826fc976d9b0f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'trajetoria)))
  "Returns md5sum for a message object of type 'trajetoria"
  "2cb8d6f7fd8051a8b0b826fc976d9b0f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<trajetoria>)))
  "Returns full string definition for message of type '<trajetoria>"
  (cl:format cl:nil "geometry_msgs/Pose2D[] pontos~%~%================================================================================~%MSG: geometry_msgs/Pose2D~%# This expresses a position and orientation on a 2D manifold.~%~%float64 x~%float64 y~%float64 theta~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'trajetoria)))
  "Returns full string definition for message of type 'trajetoria"
  (cl:format cl:nil "geometry_msgs/Pose2D[] pontos~%~%================================================================================~%MSG: geometry_msgs/Pose2D~%# This expresses a position and orientation on a 2D manifold.~%~%float64 x~%float64 y~%float64 theta~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <trajetoria>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'pontos) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <trajetoria>))
  "Converts a ROS message object to a list"
  (cl:list 'trajetoria
    (cl:cons ':pontos (pontos msg))
))
