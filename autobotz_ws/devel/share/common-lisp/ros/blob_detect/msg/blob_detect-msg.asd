
(cl:in-package :asdf)

(defsystem "blob_detect-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "feature" :depends-on ("_package_feature"))
    (:file "_package_feature" :depends-on ("_package"))
  ))