
(cl:in-package :asdf)

(defsystem "estrategia-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "trajetoria" :depends-on ("_package_trajetoria"))
    (:file "_package_trajetoria" :depends-on ("_package"))
    (:file "velocidade" :depends-on ("_package_velocidade"))
    (:file "_package_velocidade" :depends-on ("_package"))
  ))