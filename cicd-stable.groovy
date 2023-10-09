
node('linux')
{
  stage ('Poll') {
                // Poll for local changes
                checkout([
                        $class: 'GitSCM',
                        branches: [[name: '*/main']],
                        doGenerateSubmoduleConfigurations: false,
                        extensions: [],
                        userRemoteConfigs: [[url: 'https://github.com/ZOSOpenTools/vimport.git']]])
  }

  stage('Build') {
    build job: 'Port-Pipeline', parameters: [
      string(name: 'PORT_GITHUB_REPO', value: 'https://github.com/ZOSOpenTools/vimport.git'), 
      string(name: 'PORT_DESCRIPTION', value: 'vim editor' ),
      string(name: 'BUILD_LINE', value: 'STABLE')
    ]
  }
}

