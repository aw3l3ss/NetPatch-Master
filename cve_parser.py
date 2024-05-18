import requests
import re
import argparse

def find_cpe(component, version):
    base_url = "https://nvd.nist.gov/products/cpe/search/results"
    params = {
        "namingFormat": "2.3",
        "keyword": f"{component} {version}"
    }

    response = requests.get(base_url, params=params)
    content = response.text

    cpe_matches = re.findall(r'cpe:(.*?)<', content)

    return cpe_matches[0]

def find_cve(vendor, product, version):
    base_url = "https://nvd.nist.gov/vuln/search/results"
    params = {
        "form_type": "Advanced",
        "results_type": "overview",
        "search_type": "all",
        "isCpeNameSearch": "false",
        "cpe_vendor": f"cpe:/:{vendor}",
        "cpe_product": f"cpe:/::{product}",
        "cpe_version": f"cpe:/:{vendor}:{product}:{version}"
    }

    response = requests.get(base_url, params=params)
    content = response.text

    cve_matches = [0] * 5

    cve_matches[0] = re.findall(r'CVE-(.*?)<\/a>', content)
    cve_matches[1] = re.findall(r'vuln-summary-(.*?)">((.|\s)*?)<\/p>', content)

    for i in range(0, len(cve_matches[0])):
        cve_matches[1][i] = cve_matches[1][i][1]

    cvss = re.findall(r'vuln-cvss4-link-(.*?)">(.*?)</a>', content)

    j = 0
    cvss_matches = []
    for i in range(0, len(cve_matches[0])):
        if (j < len(cvss) and int(cvss[j][0]) == i):
            cvss_matches.append(cvss[j][1])
            j += 1
        
        else:
            cvss_matches.append("N/A")
    
    cve_matches[2] = cvss_matches

    cvss = re.findall(r'vuln-cvss3-link-(.*?)">(.*?)</a>', content)

    j = 0
    cvss_matches = []
    for i in range(0, len(cve_matches[0])):
        if (j < len(cvss) and int(cvss[j][0]) == i):
            cvss_matches.append(cvss[j][1])
            j += 1
        
        else:
            cvss_matches.append("N/A")
    
    cve_matches[3] = cvss_matches

    cvss = re.findall(r'vuln-cvss2-link-(.*?)">(.*?)</a>', content)

    j = 0
    cvss_matches = []
    for i in range(0, len(cve_matches[0])):
        if (j < len(cvss) and int(cvss[j][0]) == i):
            cvss_matches.append(cvss[j][1])
            j += 1
        
        else:
            cvss_matches.append("N/A")
    
    cve_matches[4] = cvss_matches

    return cve_matches

if __name__ == "__main__":
    # component = input("Input package name: ")
    # version = input("Input version: ")

    parser = argparse.ArgumentParser(description="Parse cve")

    parser.add_argument('-c', dest="component", required=True)
    parser.add_argument('-v', dest="version", required=True)

    args = parser.parse_args()

    component = args.component
    version = args.version

    cpe_string = find_cpe(component, version)

    if cpe_string:
        cpe_string = cpe_string[6:]

        vendor = cpe_string[:cpe_string.find(':')]

        cpe_string = cpe_string[cpe_string.find(':') + 1:]

        product = cpe_string[:cpe_string.find(':')]

        cpe_string = cpe_string[cpe_string.find(':') + 1:]

        version = cpe_string[:cpe_string.find(':')]

        cve_strings = find_cve(vendor, product, version)

        print()
        for i in range(0, len(cve_strings[0])):
            print("Vuln id: CVE-" + cve_strings[0][i])
            print("Summary: " + cve_strings[1][i])
            print("CVSS Severity:")
            print("V4.0: " + cve_strings[2][i])
            print("V3.1: " + cve_strings[3][i])
            print("V2.0: " + cve_strings[4][i])
            print()

    else:
        print("No CPE found for the provided component and version.")