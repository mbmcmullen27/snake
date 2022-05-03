from kubernetes import client, config, utils
import sys

def main():
    print(sys.argv[0])
    config.load_kube_config()
    k8s_client = client.ApiClient()
    yaml_file = "bundle/{}".format(sys.argv[0])
    print(yaml_file)
    utils.create_from_yaml(k8s_client, yaml_file)

if __name__ == '__main__':
    main()