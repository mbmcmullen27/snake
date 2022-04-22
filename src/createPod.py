import yaml
import urllib3
urllib3.disable_warnings()

from os import path
from kubernetes import client, config


def main():
    config.load_kube_config()

    with open(path.join(path.dirname(__file__), "blinky.yaml")) as f:
        dep = yaml.safe_load(f)
        k8s_apps_v1 = client.CoreV1Api()
        resp = k8s_apps_v1.create_namespaced_pod(body=dep, namespace="default")
        print("pod created. status='%s'" % resp.metadata.name)

if __name__ == '__main__':
    main()