; Auto-generated. Do not edit!


(cl:in-package estrategia-msg)


;//! \htmlinclude velocidade.msg.html

(cl:defclass <velocidade> (roslisp-msg-protocol:ros-message)
  ((linear
    :reader linear
    :initarg :linear
    :type std_msgs-msg:Float32
    :initform (cl:make-instance 'std_msgs-msg:Float32))
   (angular
    :reader angular
    :initarg :angular
    :type std_msgs-msg:Float32
    :initform (cl:make-instance 'std_msgs-msg:Float32)))
)

(cl:defclass velocidade (<velocidade>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <velocidade>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'velocidade)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name estrategia-msg:<velocidade> is deprecated: use estrategia-msg:velocidade instead.")))

(cl:ensure-generic-function 'linear-val :lambda-list '(m))
(cl:defmethod linear-val ((m <velocidade>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader estrategia-msg:linear-val is deprecated.  Use estrategia-msg:linear instead.")
  (linear m))

(cl:ensure-generic-function 'angular-val :lambda-list '(m))
(cl:defmethod angular-val ((m <velocidade>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader estrategia-msg:angular-val is deprecated.  Use estrategia-msg:angular instead.")
  (angular m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <velocidade>) ostream)
  "Serializes a message object of type '<velocidade>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'linear) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'angular) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <velocidade>) istream)
  "Deserializes a message object of type '<velocidade>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'linear) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'angular) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<velocidade>)))
  "Returns string type for a message object of type '<velocidade>"
  "estrategia/velocidade")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'velocidade)))
  "Returns string type for a message object of type 'velocidade"
  "estrategia/velocidade")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<velocidade>)))
  "Returns md5sum for a message object of type '<velocidade>"
  "624d0bd1a9116896d149b60f7e32ef01")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'velocidade)))
  "Returns md5sum for a message object of type 'velocidade"
  "624d0bd1a9116896d149b60f7e32ef01")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<velocidade>)))
  "Returns full string definition for message of type '<velocidade>"
  (cl:format cl:nil "std_msgs/Float32 linear~%std_msgs/Float32 angular~%~%================================================================================~%MSG: std_msgs/Float32~%float32 data~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'velocidade)))
  "Returns full string definition for message of type 'velocidade"
  (cl:format cl:nil "std_msgs/Float32 linear~%std_msgs/Float32 angular~%~%================================================================================~%MSG: std_msgs/Float32~%float32 data~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <velocidade>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'linear))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'angular))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <velocidade>))
  "Converts a ROS message object to a list"
  (cl:list 'velocidade
    (cl:cons ':linear (linear msg))
    (cl:cons ':angular (angular msg))
))
