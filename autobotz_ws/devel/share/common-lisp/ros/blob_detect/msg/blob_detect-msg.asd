
(cl:in-package :asdf)

(defsystem "blob_detect-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "squareCenters" :depends-on ("_package_squareCenters"))
    (:file "_package_squareCenters" :depends-on ("_package"))
  ))