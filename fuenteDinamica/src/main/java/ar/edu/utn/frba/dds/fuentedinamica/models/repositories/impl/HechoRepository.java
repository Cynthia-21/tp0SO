package ar.edu.utn.frba.dds.fuentedinamica.models.repositories.impl;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuentedinamica.models.repositories.IHechoRepository;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;

@Repository
public class HechoRepository implements IHechoRepository {
  private List<Hecho> hechos;

  public HechoRepository(){
    this.hechos = new ArrayList<>();
  }


  @Override
  public Hecho save(Hecho hecho) {
    if (hecho.getId() == null) {
      hecho.setId((long) this.hechos.size());
    } else {
      this.hechos.remove(this.findById(hecho.getId()));
    }
    this.hechos.add(hecho);

    return hecho;
  }

  @Override
  public List<Hecho> findAll() {
    return this.hechos;
  }

  @Override
  public Hecho findById(Long id) {
    return this.hechos.stream().filter(hecho -> hecho.getId().equals(id)).findFirst().orElse(null);
  }

  @Override
  public void delete(Hecho hecho) {
    this.hechos.remove(hecho);
  }
}
