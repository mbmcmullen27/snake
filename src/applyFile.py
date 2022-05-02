from kubernetes import client, config, utils
import sys

def main():
    if len(sys.argv) != 2:
        sys.exit("Too few arguments")
    config.load_kube_config()
    k8s_client = client.ApiClient()
    yaml_file = sys.argv[1]
    utils.create_from_yaml(k8s_client, yaml_file)

if __name__ == '__main__':
    main()