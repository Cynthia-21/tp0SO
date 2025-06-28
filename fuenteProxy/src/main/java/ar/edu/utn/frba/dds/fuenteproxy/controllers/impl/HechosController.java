package ar.edu.utn.frba.dds.fuenteproxy.controllers.impl;

import ar.edu.utn.frba.dds.fuenteproxy.models.dtos.input.MetamapaResponseDTO;
import ar.edu.utn.frba.dds.fuenteproxy.models.dtos.output.HechoOutputDTO;
import ar.edu.utn.frba.dds.fuenteproxy.models.entities.Hecho;
import ar.edu.utn.frba.dds.fuenteproxy.models.entities.fuentes.Fuente;
import ar.edu.utn.frba.dds.fuenteproxy.services.impl.ApiServiceLocator;
import ar.edu.utn.frba.dds.fuenteproxy.services.impl.HechoService;
import ar.edu.utn.frba.dds.fuenteproxy.services.IFuenteService;
import ar.edu.utn.frba.dds.fuenteproxy.services.impl.SeederService;
import ar.edu.utn.frba.dds.fuenteproxy.services.impl.apiService.IapiAConsumirService;
import java.util.List;
import java.util.stream.Collectors;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/servicioFuenteProxy")
public class HechosController {
  private final SeederService seederService;
  private final IFuenteService fuenteService;
  private final HechoService hechoService;
  private final ApiServiceLocator serviceLocator;
  public HechosController(SeederService seederService, HechoService hechoService, IFuenteService fuenteService,ApiServiceLocator serviceLocator) {
    this.seederService = seederService;
    this.serviceLocator = serviceLocator;
    this.fuenteService = fuenteService;
    this.hechoService = hechoService;
  }
  @GetMapping("/iniciarRepoFuentes")
  public String iniciarRepoFuentes(){
    seederService.init();
    return "Inicio de repositorio de pruebas exitoso";
  }
  @GetMapping("/{idFuente}/hechos")
  public List<HechoOutputDTO> getAllHechos(
      @PathVariable String idFuente,
      @RequestParam(required = false) Integer page,
      @RequestParam(required = false) Integer per_page){
    Fuente fuente = fuenteService.buscarPorID(Long.valueOf(idFuente));
    IapiAConsumirService servicio = serviceLocator.obtenerServicioDeFuente(fuente);

    if (page != null && per_page != null) {
      return servicio.getHechosPorPagina(page, per_page, fuente);
    } else {
      return servicio.getAllHechos(fuente);
    }
    //return serviceLocator.obtenerServicioDeFuente(fuente).getAllHechos(fuente);
  }
  @GetMapping("/miServicio")
  public MetamapaResponseDTO obtenerHechosLocales(){
    MetamapaResponseDTO response = new MetamapaResponseDTO();
    List<HechoOutputDTO> hechos = hechoService.buscarTodos()
        .stream()
        .map(this::hechoOutputDTO)
        .collect(Collectors.toList()); // Si es un Stream
    response.setHechos(hechos);
    return response;
  }
  public HechoOutputDTO hechoOutputDTO(Hecho input){
    HechoOutputDTO hechoDTO = new HechoOutputDTO();
    //todo Por ahora hecho output dto es igual al input de la api, falta darle el formato que queramos
    hechoDTO.setId(input.getId());
    hechoDTO.setTitulo(input.getTitulo());
    hechoDTO.setDescripcion(input.getDescripcion());
    return hechoDTO;
  }
}
