import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import fs from 'fs'
import path from 'path'

function ferrosApiPlugin() {
  return {
    name: 'ferros-api',
    configureServer(server: any) {
      server.middlewares.use('/api/insights', (_req: any, res: any) => {
        const brokerOutputDir = path.resolve(__dirname, '../Broker/output')
        const insights: any[] = []

        // Recursive function to get all JSON files
        function walkSync(dir: string) {
          if (!fs.existsSync(dir)) return
          const files = fs.readdirSync(dir)
          for (const file of files) {
            const filepath = path.join(dir, file)
            const stat = fs.statSync(filepath)
            if (stat.isDirectory()) {
              walkSync(filepath)
            } else if (file.endsWith('.json')) {
              try {
                const content = fs.readFileSync(filepath, 'utf-8')
                insights.push(JSON.parse(content))
              } catch (e) {
                console.error(`Error parsing ${filepath}`, e)
              }
            }
          }
        }

        walkSync(brokerOutputDir)
        
        res.setHeader('Content-Type', 'application/json')
        res.end(JSON.stringify(insights))
      })
    }
  }
}

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react(), ferrosApiPlugin()],
})
