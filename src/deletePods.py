import yaml
import os
from kubernetes import client, config

def main():
    config.load_kube_config()

    label_selector = 'deployed=kube-snake'

    k8s_apps_v1 = client.CoreV1Api()
    ret = k8s_apps_v1.list_namespaced_pod("default", label_selector=label_selector)
    print("Found {d} pods".format(d=len(ret.items)))
    body = client.V1DeleteOptions()
    for p in ret.items:
        k8s_apps_v1.delete_namespaced_pod(p.metadata.name, "default", body=body) 

if __name__ == '__main__':
    main()