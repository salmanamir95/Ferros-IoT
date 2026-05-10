import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import fs from 'fs'
import path from 'path'

function ferrosApiPlugin() {
  const fileCache = new Map<string, { mtimeMs: number, data: any }>();

  return {
    name: 'ferros-api',
    configureServer(server: any) {
      server.middlewares.use('/api/insights', (_req: any, res: any) => {
        const brokerOutputDir = path.resolve(__dirname, '../Broker/output')
        const insights: any[] = []

        // Recursive function to get all JSON files
        function walkSync(dir: string) {
          if (!fs.existsSync(dir)) return
          const files = fs.readdirSync(dir, { withFileTypes: true })
          
          for (const dirent of files) {
            const filepath = path.join(dir, dirent.name)
            
            if (dirent.isDirectory()) {
              walkSync(filepath)
            } else if (dirent.name.endsWith('.json')) {
              // If we already parsed this exact file, just serve from cache!
              // Ferros creates immutable timestamp.json files, so we don't need to re-read or stat them.
              if (fileCache.has(filepath)) {
                insights.push(fileCache.get(filepath)!.data)
              } else {
                try {
                  const content = fs.readFileSync(filepath, 'utf-8')
                  const data = JSON.parse(content)
                  fileCache.set(filepath, { mtimeMs: 0, data }) // mtime doesn't matter for immutable files
                  insights.push(data)
                } catch (e) {
                  // File might be currently being written
                }
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
