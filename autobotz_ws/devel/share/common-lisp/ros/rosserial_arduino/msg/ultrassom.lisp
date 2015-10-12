; Auto-generated. Do not edit!


(cl:in-package rosserial_arduino-msg)


;//! \htmlinclude ultrassom.msg.html

(cl:defclass <ultrassom> (roslisp-msg-protocol:ros-message)
  ((dir
    :reader dir
    :initarg :dir
    :type cl:float
    :initform 0.0)
   (esq
    :reader esq
    :initarg :esq
    :type cl:float
    :initform 0.0)
   (frente
    :reader frente
    :initarg :frente
    :type cl:float
    :initform 0.0)
   (tras
    :reader tras
    :initarg :tras
    :type cl:float
    :initform 0.0))
)

(cl:defclass ultrassom (<ultrassom>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ultrassom>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ultrassom)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rosserial_arduino-msg:<ultrassom> is deprecated: use rosserial_arduino-msg:ultrassom instead.")))

(cl:ensure-generic-function 'dir-val :lambda-list '(m))
(cl:defmethod dir-val ((m <ultrassom>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rosserial_arduino-msg:dir-val is deprecated.  Use rosserial_arduino-msg:dir instead.")
  (dir m))

(cl:ensure-generic-function 'esq-val :lambda-list '(m))
(cl:defmethod esq-val ((m <ultrassom>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rosserial_arduino-msg:esq-val is deprecated.  Use rosserial_arduino-msg:esq instead.")
  (esq m))

(cl:ensure-generic-function 'frente-val :lambda-list '(m))
(cl:defmethod frente-val ((m <ultrassom>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rosserial_arduino-msg:frente-val is deprecated.  Use rosserial_arduino-msg:frente instead.")
  (frente m))

(cl:ensure-generic-function 'tras-val :lambda-list '(m))
(cl:defmethod tras-val ((m <ultrassom>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rosserial_arduino-msg:tras-val is deprecated.  Use rosserial_arduino-msg:tras instead.")
  (tras m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ultrassom>) ostream)
  "Serializes a message object of type '<ultrassom>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'dir))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'esq))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'frente))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'tras))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ultrassom>) istream)
  "Deserializes a message object of type '<ultrassom>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'dir) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'esq) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'frente) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'tras) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ultrassom>)))
  "Returns string type for a message object of type '<ultrassom>"
  "rosserial_arduino/ultrassom")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ultrassom)))
  "Returns string type for a message object of type 'ultrassom"
  "rosserial_arduino/ultrassom")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ultrassom>)))
  "Returns md5sum for a message object of type '<ultrassom>"
  "fb6e69afd8489e9696f90b5f332c934f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ultrassom)))
  "Returns md5sum for a message object of type 'ultrassom"
  "fb6e69afd8489e9696f90b5f332c934f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ultrassom>)))
  "Returns full string definition for message of type '<ultrassom>"
  (cl:format cl:nil "float32 dir~%float32 esq~%float32 frente~%float32 tras~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ultrassom)))
  "Returns full string definition for message of type 'ultrassom"
  (cl:format cl:nil "float32 dir~%float32 esq~%float32 frente~%float32 tras~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ultrassom>))
  (cl:+ 0
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ultrassom>))
  "Converts a ROS message object to a list"
  (cl:list 'ultrassom
    (cl:cons ':dir (dir msg))
    (cl:cons ':esq (esq msg))
    (cl:cons ':frente (frente msg))
    (cl:cons ':tras (tras msg))
))
